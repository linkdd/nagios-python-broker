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

#include "module.h"
#include "logging.h"
#include "nebmodule.h"
#include <Python.h>

static int nebmodule_callback (int event_type, void *data)
{
    PyObject *module = PyImport_ImportModule ("nagios_python_broker");
    PyObject *handle = PyObject_GetAttrString (module, "handle");
    PyObject *ret = PyObject_CallMethod (
        handle, "__call__",
        "iO&", event_type, data
    );

    int retval = PyInt_AsLong (ret);
    
    Py_DECREF (ret);
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

static int nebmodule_main (PyObject *module)
{
    NebModule *handle = (NebModule *) PyObject_GetAttrString (module, "handle");

    nebmodule_register (handle);

    Py_DECREF (handle);

    return 0;
}

int nebmodule_init (
    int flags __attribute__ ((__unused__)),
    char *args,
    nebmodule *handle)
{
    PyObject *module = NULL;
    PyObject *usermodule = NULL;
    PyObject *UserNebModule = NULL;
    PyObject *arguments = NULL;
    PyObject *pyhandle = NULL;

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
    NebModuleType_Initialize (module);

    /* import user module in our package */
    usermodule = PyImport_ImportModule (args);

    if (usermodule == NULL)
    {
        nebmodule_log_error ("Impossible to load user module");
        nebmodule_log_exception ();
        return -1;
    }

    PyModule_AddObject (module, "usermodule", usermodule);

    nebmodule_log_int ("Initialize Python NEB module");

    UserNebModule = PyObject_GetAttrString (usermodule, "get_handle");
    Py_DECREF (usermodule);

    arguments = Py_BuildValue ("(O&)", handle);
    pyhandle = PyObject_CallObject (UserNebModule, arguments);
    Py_DECREF (arguments);
    Py_DECREF (UserNebModule);

    PyModule_AddObject (module, "handle", pyhandle);
    Py_DECREF (pyhandle);

    return nebmodule_main (module);
}

int nebmodule_deinit (
    int flags __attribute__ ((__unused__)),
    int reason __attribute__ ((__unused__)))
{
    nebmodule_log_info ("nagios-python-broker deinitializing");

    nebmodule_deregister ();
    Py_Finalize ();
}
