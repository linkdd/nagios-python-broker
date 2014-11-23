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

#include "nagoshost.h"
#include "pyclassmacros.h"
#include <structmember.h>

/* methods */
static void NagiosHost_dealloc (NagiosHost *self);
static PyObject *NagiosHost_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NagiosHost_init (NagiosHost *self, PyObject *args, PyObject *kwargs);

/* properties */
PYCLASS_DECL_GETPROP (NagiosHost, name);
PYCLASS_DECL_GETPROP (NagiosHost, display_name);
PYCLASS_DECL_GETPROP (NagiosHost, alias);
PYCLASS_DECL_GETPROP (NagiosHost, address);
PYCLASS_DECL_GETPROP (NagiosHost, parents);
PYCLASS_DECL_GETPROP (NagiosHost, children);
PYCLASS_DECL_GETPROP (NagiosHost, services);
PYCLASS_DECL_GETPROP (NagiosHost, check_command);
PYCLASS_DECL_GETPROP (NagiosHost, initial_state);
PYCLASS_DECL_GETPROP (NagiosHost, check_interval);
PYCLASS_DECL_GETPROP (NagiosHost, retry_interval);
PYCLASS_DECL_GETPROP (NagiosHost, max_attempts);
PYCLASS_DECL_GETPROP (NagiosHost, event_handler);
PYCLASS_DECL_GETPROP (NagiosHost, contact_groups);
PYCLASS_DECL_GETPROP (NagiosHost, contacts);
PYCLASS_DECL_GETPROP (NagiosHost, notification_interval);
PYCLASS_DECL_GETPROP (NagiosHost, first_notification_delay);
PYCLASS_DECL_GETPROP (NagiosHost, notify_on_down);
PYCLASS_DECL_GETPROP (NagiosHost, notify_on_unreachable);
PYCLASS_DECL_GETPROP (NagiosHost, notify_on_recovery);
PYCLASS_DECL_GETPROP (NagiosHost, notify_on_flapping);
PYCLASS_DECL_GETPROP (NagiosHost, notify_on_downtime);
PYCLASS_DECL_GETPROP (NagiosHost, notification_period);

void NagiosHostType_Initialize (PyObject *namespace);
PyObject *NagiosHost_New (host *data);

PyObject *NagiosHost_AddParent (PyObject *self, PyObject *hostname);
PyObject *NagiosHost_AddChild (PyObject *self, PyObject *childhost);
PyObject *NagiosHost_AddContactGroup (PyObject *self, PyObject *cgroupname);
PyObject *NagiosHost_AddContact (PyObject *self, PyObject *contactname);
PyObject *NagiosHost_AddCustomVariable (PyObject *self, PyObject *varname, PyObject *value);
PyObject *NagiosHost_AddServiceLink (PyObject *self, PyObject *servicelink);

/* vtables */
static PyMethodDef NagiosHost_methods[] =
{
    { NULL }
};

static PyMemberDef NagiosHost_members[] =
{
    { NULL }
};

static PyGetSetDef NagiosHost_getseters[] =
{
    PYCLASS_ADD_PROP("name", PYCLASS_GETPROP (NagiosHost, name), NULL, "host.name"),
    PYCLASS_ADD_PROP("display_name", PYCLASS_GETPROP (NagiosHost, display_name), NULL, "host.display_name"),
    PYCLASS_ADD_PROP("alias", PYCLASS_GETPROP (NagiosHost, alias), NULL, "host.alias"),
    PYCLASS_ADD_PROP("address", PYCLASS_GETPROP (NagiosHost, address), NULL, "host.address"),
    PYCLASS_ADD_PROP("parents", PYCLASS_GETPROP (NagiosHost, parents), NULL, "host.parent_hosts"),
    PYCLASS_ADD_PROP("children", PYCLASS_GETPROP (NagiosHost, children), NULL, "host.child_hosts"),
    PYCLASS_ADD_PROP("services", PYCLASS_GETPROP (NagiosHost, services), NULL, "host.services"),
    PYCLASS_ADD_PROP("check_command", PYCLASS_GETPROP (NagiosHost, check_command), NULL, "host.host_check_command"),
    PYCLASS_ADD_PROP("initial_state", PYCLASS_GETPROP (NagiosHost, initial_state), NULL, "host.initial_state"),
    { NULL }
};

PyTypeObject NagiosHostType =
{
    PyObject_HEAD_INIT (NULL)
    0,
    "NagiosHost",
    sizeof (NagiosHost),
    0,                                  /* tp_itemsize */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash  */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    "NagiosHost wrapper",
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NagiosHost_methods,
    NagiosHost_members,
    NagiosHost_getseters,
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) NagiosHost_init,
    0,                                  /* tp_alloc */
    NagiosHost_new
};

/* implementation */

void NagiosHostType_Initialize (PyObject *namespace)
{
    if (PyType_Ready (&NagiosHostType) < 0)
    {
        return;
    }

    Py_INCREF (&NagiosHostType);
    PyModule_AddObject (namespace, "NagiosHost", (PyObject *) (&NagiosHostType));
}

static void NagiosHost_dealloc (NagiosHost *self)
{
    self->ob_type->tp_free ((PyObject *) self);
}

static PyObject *NagiosHost_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    NagiosHost *self = (NagiosHost *) type->tp_alloc (type, 0);

    if (self != NULL)
    {
        self->handle = NULL;
    }

    return (PyObject *) self;
}

static int NagiosHost_init (NagiosHost *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords (args, kwargs, "|O&", kwlist, &(self->data)))
    {
        return -1;
    }

    return 0;
}

PYCLASS_DEF_GETPROP (NagiosHost, name, data->name, PyString_FromString)
PYCLASS_DEF_GETPROP (NagiosHost, display_name, data->display_name, PyString_FromString)
PYCLASS_DEF_GETPROP (NagiosHost, alias, data->alias, PyString_FromString)
PYCLASS_DEF_GETPROP (NagiosHost, address, data->address, PyString_FromString)

PYCLASS_DEF_GETPROP_WITH_CODE (NagiosHost, parents)
{
    return PyObject_GetAttrString ((PyObject *) self, "_parents");
}

PYCLASS_DEF_GETPROP_WITH_CODE (NagiosHost, children)
{
    return PyObject_GetAttrString ((PyObject *) self, "_children");
}

PYCLASS_DEF_GETPROP_WITH_CODE (NagiosHost, services)
{
    return PyObject_GetAttrString ((PyObject *) self, "_services");
}
