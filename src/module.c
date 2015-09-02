/* The MIT License (MIT)
 *
 * Copyright (c) 2014 David Delassus <david.jose.delassus@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Python.h>

#include "module.h"
#include "logging.h"

#include "nebmodule.h"
#include <generated/nebstructs/nebstructs.h>

static PyObject *nebmodule_init_constants (PyObject *namespace)
{
    char *callback_names[NEBCALLBACK_NUMITEMS] =
    {
        "handle_reserved0",
        "handle_reserved1",
        "handle_reserved2",
        "handle_reserved3",
        "handle_reserved4",
        "handle_raw",
        "handle_neb",
        "handle_process",
        "handle_timed_event",
        "handle_log",
        "handle_system_command",
        "handle_event_handler",
        "handle_notification",
        "handle_service_check",
        "handle_host_check",
        "handle_comment",
        "handle_downtime",
        "handle_flapping",
        "handle_program_status",
        "handle_host_status",
        "handle_service_status",
        "handle_adaptive_program",
        "handle_adaptive_host",
        "handle_adaptive_service",
        "handle_external_command",
        "handle_aggregated_status",
        "handle_retention",
        "handle_contact_notification",
        "handle_contact_notification_method",
        "handle_acknowledgement",
        "handle_state_change",
        "handle_contact_status",
        "handle_adaptive_contact"
    };

    PyObject *nebcallbacks = PyDict_New ();
    int nebcallback;

    for (nebcallback = 0 ; nebcallback < NEBCALLBACK_NUMITEMS ; ++nebcallback)
    {
        PyObject *key = PyInt_FromLong (nebcallback);
        PyObject *value = PyString_FromString (callback_names[nebcallback]);

        PyDict_SetItem (nebcallbacks, key, value);
        PyDict_SetItem (nebcallbacks, value, key);

        Py_DECREF (value);
        Py_DECREF (key);
    }

    PyModule_AddObject (namespace, "nebcallbacks", nebcallbacks);
    Py_DECREF (nebcallbacks);

    return Py_True;
}

static PyObject *nebmodule_init_register (PyObject *namespace)
{
    PyObject *register_ = PyDict_New ();

    PyModule_AddObject (namespace, "register", register_);

    return Py_True;
}

static PyObject *nebmodule_init_types (PyObject *namespace)
{
    typedef void (*inittype) (PyObject *);

    inittype initializers[] =
    {
        NebModuleType_Initialize,
        NebStructTypes_Initialize,
        NULL
    };

    int i;

    for (i = 0; initializers[i] != NULL; ++i)
    {
        initializers[i] (namespace);
    }

    return Py_True;
}

static PyObject *nebmodule_init_usermodule (
    PyObject *namespace,
    const char *package,
    nebmodule *nebhandle)
{
    /* import user module in our package */
    PyObject *usermodule = PyImport_ImportModule (package);
    PyObject *get_handle = NULL, *arguments = NULL;
    PyObject *handle = NULL;

    if (usermodule == NULL)
    {
        nebmodule_log_exception ();
        return Py_False;
    }

    PyModule_AddObject (namespace, "usermodule", usermodule);

    nebmodule_log_info ("Initialize Python NEB module");

    get_handle = PyObject_GetAttrString (usermodule, "get_handle");
    Py_DECREF (usermodule);

    arguments = Py_BuildValue ("(O&)", nebhandle);
    handle = PyObject_CallObject (get_handle, arguments);
    Py_DECREF (arguments);
    Py_DECREF (get_handle);

    PyModule_AddObject (namespace, "handle", handle);
    Py_DECREF (handle);

    return Py_True;
}

static int nebmodule_callback (int event_type, void *data)
{
    PyObject *ret = NULL;
    int retval = 0;

    PyObject *module = PyImport_ImportModule ("nagios_python_broker");
    PyObject *handle = PyObject_GetAttrString (module, "handle");

    /* fetch event handler */
    PyObject *nebcallbacks = PyObject_GetAttrString (module, "nebcallbacks");
    PyObject *nebcallback = PyInt_FromLong (event_type);
    
    PyObject *evhandler = PyDict_GetItem (nebcallbacks, nebcallback);
    char *evhandlerstr = PyString_AsString (evhandler);

    Py_DECREF (evhandler);
    Py_DECREF (nebcallback);
    Py_DECREF (nebcallbacks);

    /* execute handler */
    if (PyObject_HasAttrString (handle, evhandlerstr))
    {
        ret = PyObject_CallMethod (handle, evhandlerstr, "O&", data);
    }
    else
    {
        PyErr_Format (
            PyExc_NotImplementedError,
            "No handler for event type: %s",
            evhandlerstr
        );
    }

    if (ret == NULL)
    {
        nebmodule_log_exception ();
        retval = -1;
    }
    else
    {
        retval = PyInt_AsLong (ret);
        Py_DECREF (ret);
    }

    Py_DECREF (handle);
    Py_DECREF (module);

    return retval;
}

static void nebmodule_register (NebModule *pyhandle)
{
    nebmodule *handle = NebModule_GetHandle (pyhandle);
    int nebcallback;

    nebmodule_log_info ("Register callbacks");

    /* NEB callbacks are identified as int by macros, 0 to 4 is reserved */
    for (nebcallback = 5 ; nebcallback < NEBCALLBACK_NUMITEMS ; ++nebcallback)
    {
        (void) neb_register_callback (nebcallback, handle, 0, nebmodule_callback);
    }
}

static void nebmodule_deregister (void)
{
    int nebcallback;

    nebmodule_log_info ("Deregister callbacks");

    /* NEB callbacks are identified as int by macros, 0 to 4 is reserved */
    for (nebcallback = 5 ; nebcallback < NEBCALLBACK_NUMITEMS ; ++nebcallback)
    {
        (void) neb_deregister_callback (nebcallback, nebmodule_callback);
    }
}

static PyObject *nebmodule_main (PyObject *module)
{
    NebModule *handle = (NebModule *) PyObject_GetAttrString (module, "handle");

    nebmodule_register (handle);

    Py_DECREF (handle);

    return Py_True;
}

int nebmodule_init (
    int flags __attribute__ ((__unused__)),
    char *args,
    nebmodule *handle)
{
    PyObject *module = NULL;

    Py_SetProgramName ("nagios-python-broker");
    Py_Initialize ();

    nebmodule_log_info ("nagios-python-broker initializing...");

    /* create python module */
    module = PyModule_New ("nagios_python_broker");

    if (module == NULL)
    {
        nebmodule_log_error ("Impossible to create nagios_python_broker module");
        return -1;
    }

    /* initializes all data-types in our python module */
    if (nebmodule_init_constants (module) == Py_False)
    {
        nebmodule_log_error ("Impossible to initialize constants");
        return -1;
    }

    if (nebmodule_init_register (module) == Py_False)
    {
        nebmodule_log_error ("Impossible to initialize register");
        return -1;
    }

    if (nebmodule_init_types (module) == Py_False)
    {
        nebmodule_log_error ("Impossible to initialize types");
        return -1;
    }

    if (nebmodule_init_usermodule (module, args, handle) == Py_False)
    {
        nebmodule_log_error ("Impossible to initialize module: %s", args);
        return -1;
    }

    if (nebmodule_main (module) == Py_False)
    {
        nebmodule_log_exception ();
        return -1;
    }

    return 0;
}

int nebmodule_deinit (
    int flags __attribute__ ((__unused__)),
    int reason __attribute__ ((__unused__)))
{
    nebmodule_log_info ("nagios-python-broker deinitializing");

    nebmodule_deregister ();
    Py_Finalize ();
}
