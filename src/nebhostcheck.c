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

#include "nebhostcheck.h"
#include "pyclassmacros.h"
/* #include "nagioshost.h" -- not yet implemented */
#include <structmember.h>

/* methods */
static void NebHostCheck_dealloc (NebHostCheck *self);
static PyObject *NebHostCheck_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NebHostCheck_init (NebHostCheck *self, PyObject *args, PyObject *kwargs);

/* properties */
PYCLASS_DECL_GETPROP (NebHostCheck, host);
PYCLASS_DECL_GETPROP (NebHostCheck, hostname);
PYCLASS_DECL_GETPROP (NebHostCheck, current_attempt);
PYCLASS_DECL_GETPROP (NebHostCheck, max_attempts);
PYCLASS_DECL_GETPROP (NebHostCheck, check_type);
PYCLASS_DECL_GETPROP (NebHostCheck, state_type);
PYCLASS_DECL_GETPROP (NebHostCheck, state);
PYCLASS_DECL_GETPROP (NebHostCheck, command_name);
PYCLASS_DECL_GETPROP (NebHostCheck, command_args);
PYCLASS_DECL_GETPROP (NebHostCheck, command_line);
PYCLASS_DECL_GETPROP (NebHostCheck, start_time);
PYCLASS_DECL_GETPROP (NebHostCheck, end_time);
PYCLASS_DECL_GETPROP (NebHostCheck, exec_time);
PYCLASS_DECL_GETPROP (NebHostCheck, latency);
PYCLASS_DECL_GETPROP (NebHostCheck, timeout);
PYCLASS_DECL_GETPROP (NebHostCheck, early_timeout);
PYCLASS_DECL_GETPROP (NebHostCheck, return_code);
PYCLASS_DECL_GETPROP (NebHostCheck, output);
PYCLASS_DECL_GETPROP (NebHostCheck, long_output);
PYCLASS_DECL_GETPROP (NebHostCheck, perfdata);

/* vtables */
static PyMethodDef NebHostCheck_methods[] =
{
    { NULL }
};

static PyMemberDef NebHostCheck_members[] =
{
    { NULL }
};

static PyGetSetDef NebHostCheck_getseters[] =
{
    PYCLASS_ADD_PROP ("host", PYCLASS_GETPROP (NebHostCheck, host), NULL, "nebmodule.host"),
    PYCLASS_ADD_PROP ("hostname", PYCLASS_GETPROP (NebHostCheck, hostname), NULL, "nebmodule.hostname"),
    PYCLASS_ADD_PROP ("current_attempt", PYCLASS_GETPROP (NebHostCheck, current_attempt), NULL, "nebmodule.current_attempt"),
    PYCLASS_ADD_PROP ("max_attempts", PYCLASS_GETPROP (NebHostCheck, max_attempts), NULL, "nebmodule.max_attempts"),
    PYCLASS_ADD_PROP ("check_type", PYCLASS_GETPROP (NebHostCheck, check_type), NULL, "nebmodule.check_type"),
    PYCLASS_ADD_PROP ("state_type", PYCLASS_GETPROP (NebHostCheck, state_type), NULL, "nebmodule.state_type"),
    PYCLASS_ADD_PROP ("state", PYCLASS_GETPROP (NebHostCheck, state), NULL, "nebmodule.state"),
    PYCLASS_ADD_PROP ("command_name", PYCLASS_GETPROP (NebHostCheck, command_name), NULL, "nebmodule.command_name"),
    PYCLASS_ADD_PROP ("command_args", PYCLASS_GETPROP (NebHostCheck, command_args), NULL, "nebmodule.command_args"),
    PYCLASS_ADD_PROP ("command_line", PYCLASS_GETPROP (NebHostCheck, command_line), NULL, "nebmodule.command_line"),
    PYCLASS_ADD_PROP ("start_time", PYCLASS_GETPROP (NebHostCheck, start_time), NULL, "nebmodule.start_time"),
    PYCLASS_ADD_PROP ("end_time", PYCLASS_GETPROP (NebHostCheck, end_time), NULL, "nebmodule.end_time"),
    PYCLASS_ADD_PROP ("exec_time", PYCLASS_GETPROP (NebHostCheck, exec_time), NULL, "nebmodule.exec_time"),
    PYCLASS_ADD_PROP ("latency", PYCLASS_GETPROP (NebHostCheck, latency), NULL, "nebmodule.latency"),
    PYCLASS_ADD_PROP ("timeout", PYCLASS_GETPROP (NebHostCheck, timeout), NULL, "nebmodule.timeout"),
    PYCLASS_ADD_PROP ("early_timeout", PYCLASS_GETPROP (NebHostCheck, early_timeout), NULL, "nebmodule.early_timeout"),
    PYCLASS_ADD_PROP ("return_code", PYCLASS_GETPROP (NebHostCheck, return_code), NULL, "nebmodule.return_code"),
    PYCLASS_ADD_PROP ("output", PYCLASS_GETPROP (NebHostCheck, output), NULL, "nebmodule.output"),
    PYCLASS_ADD_PROP ("long_output", PYCLASS_GETPROP (NebHostCheck, long_output), NULL, "nebmodule.long_output"),
    PYCLASS_ADD_PROP ("perfdata", PYCLASS_GETPROP (NebHostCheck, perfdata), NULL, "nebmodule.perfdata"),
    { NULL }
};

PyTypeObject NebHostCheckType =
{
    PyObject_HEAD_INIT (NULL)
    0,
    "NebHostCheck",
    sizeof (NebHostCheck),
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    "nebstruct_host_check_data wrapper",
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NebHostCheck_methods,
    NebHostCheck_members,
    NebHostCheck_getseters,
    (PyTypeObject *) (&NebProcessType),
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) NebHostCheck_init,
    0,                                  /* tp_alloc */
    NebHostCheck_new
};

/* implementation */

void NebHostCheckType_Initialize (PyObject *namespace)
{
    NebProcessType_Initialize (namespace);

    if (PyType_Ready (&NebHostCheckType) < 0)
    {
        return;
    }

    Py_INCREF (&NebHostCheckType);
    PyModule_AddObject (namespace, "NebHostCheck", (PyObject *) (&NebHostCheckType));
}

static void NebHostCheck_dealloc (NebHostCheck *self)
{
    Py_XDECREF (self->host);
    NebProcessType.tp_dealloc ((PyObject *) self);
}

static PyObject *NebHostCheck_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    NebHostCheck *self = (NebHostCheck *) NebProcessType.tp_new (type, args, kwargs);

    if (self != NULL)
    {
        self->data = NULL;
        self->host = NULL;
    }

    return (PyObject *) self;
}

static int NebHostCheck_init (NebHostCheck *self, PyObject *args, PyObject *kwargs)
{
    int ret = NebProcessType.tp_init ((PyObject *) self, args, kwargs);

    if (ret == 0)
    {
        self->data = (nebstruct_host_check_data *) self->parent.data;

        /* TODO:
         * self->host = NagiosHost_New (self->data->object_ptr);
         */
    }

    return ret;
}

PYCLASS_DEF_GETPROP (NebHostCheck, host, host, Py_INCREFRET)
PYCLASS_DEF_GETPROP (NebHostCheck, hostname, data->host_name, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, current_attempt, data->current_attempt, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, max_attempts, data->max_attempts, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, check_type, data->check_type, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, state_type, data->state_type, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, state, data->state, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, command_name, data->command_name, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, command_args, data->command_args, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, command_line, data->command_line, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, start_time, data->start_time.tv_sec, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, end_time, data->end_time.tv_sec, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, exec_time, data->execution_time, PyFloat_FromDouble)
PYCLASS_DEF_GETPROP (NebHostCheck, latency, data->latency, PyFloat_FromDouble)
PYCLASS_DEF_GETPROP (NebHostCheck, timeout, data->timeout, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, early_timeout, data->early_timeout, PyBool_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, return_code, data->return_code, PyInt_FromLong)
PYCLASS_DEF_GETPROP (NebHostCheck, output, data->output, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, long_output, data->long_output, PyString_FromString)
PYCLASS_DEF_GETPROP (NebHostCheck, perfdata, data->perf_data, PyString_FromString)

PyObject *NebHostCheck_New (nebstruct_host_check_data *data)
{
    PyObject *arguments = Py_BuildValue ("(0&)", data);

    PyObject *self = PyObject_CallObject (
        (PyObject *) (&NebHostCheckType),
        arguments
    );

    Py_DECREF (arguments);

    return self;
}
