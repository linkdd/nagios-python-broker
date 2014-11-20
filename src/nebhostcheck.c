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
/* #include "nagioshost.h" -- not yet implemented */
#include <structmember.h>

/* methods */
static void NebHostCheck_dealloc (NebHostCheck *self);
static PyObject *NebHostCheck_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NebHostCheck_init (NebHostCheck *self, PyObject *args, PyObject *kwargs);

/* properties */
static PyObject *NebHostCheck_get_host (NebHostCheck *self);
static PyObject *NebHostCheck_get_hostname (NebHostCheck *self);
static PyObject *NebHostCheck_get_current_attempt (NebHostCheck *self);
static PyObject *NebHostCheck_get_max_attempts (NebHostCheck *self);
static PyObject *NebHostCheck_get_check_type (NebHostCheck *self);
static PyObject *NebHostCheck_get_state_type (NebHostCheck *self);
static PyObject *NebHostCheck_get_state (NebHostCheck *self);
static PyObject *NebHostCheck_get_command_name (NebHostCheck *self);
static PyObject *NebHostCheck_get_command_args (NebHostCheck *self);
static PyObject *NebHostCheck_get_command_line (NebHostCheck *self);
static PyObject *NebHostCheck_get_start_time (NebHostCheck *self);
static PyObject *NebHostCheck_get_end_time (NebHostCheck *self);
static PyObject *NebHostCheck_get_exec_time (NebHostCheck *self);
static PyObject *NebHostCheck_get_latency (NebHostCheck *self);
static PyObject *NebHostCheck_get_timeout (NebHostCheck *self);
static PyObject *NebHostCheck_get_early_timeout (NebHostCheck *self);
static PyObject *NebHostCheck_get_return_code (NebHostCheck *self);
static PyObject *NebHostCheck_get_output (NebHostCheck *self);
static PyObject *NebHostCheck_get_long_output (NebHostCheck *self);
static PyObject *NebHostCheck_get_perfdata (NebHostCheck *self);

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
    { "host", (getter) NebHostCheck_get_host, NULL, "nebmodule.host", NULL },
    { "hostname", (getter) NebHostCheck_get_hostname, NULL, "nebmodule.hostname", NULL },
    { "current_attempt", (getter) NebHostCheck_get_current_attempt, NULL, "nebmodule.current_attempt", NULL },
    { "max_attempts", (getter) NebHostCheck_get_max_attempts, NULL, "nebmodule.max_attempts", NULL },
    { "check_type", (getter) NebHostCheck_get_check_type, NULL, "nebmodule.check_type", NULL },
    { "state_type", (getter) NebHostCheck_get_state_type, NULL, "nebmodule.state_type", NULL },
    { "state", (getter) NebHostCheck_get_state, NULL, "nebmodule.state", NULL },
    { "command_name", (getter) NebHostCheck_get_command_name, NULL, "nebmodule.command_name", NULL },
    { "command_args", (getter) NebHostCheck_get_command_args, NULL, "nebmodule.command_args", NULL },
    { "command_line", (getter) NebHostCheck_get_command_line, NULL, "nebmodule.command_line", NULL },
    { "start_time", (getter) NebHostCheck_get_start_time, NULL, "nebmodule.start_time", NULL },
    { "end_time", (getter) NebHostCheck_get_end_time, NULL, "nebmodule.end_time", NULL },
    { "exec_time", (getter) NebHostCheck_get_exec_time, NULL, "nebmodule.exec_time", NULL },
    { "latency", (getter) NebHostCheck_get_latency, NULL, "nebmodule.latency", NULL },
    { "timeout", (getter) NebHostCheck_get_timeout, NULL, "nebmodule.timeout", NULL },
    { "early_timeout", (getter) NebHostCheck_get_early_timeout, NULL, "nebmodule.early_timeout", NULL },
    { "return_code", (getter) NebHostCheck_get_return_code, NULL, "nebmodule.return_code", NULL },
    { "output", (getter) NebHostCheck_get_output, NULL, "nebmodule.output", NULL },
    { "long_output", (getter) NebHostCheck_get_long_output, NULL, "nebmodule.long_output", NULL },
    { "perfdata", (getter) NebHostCheck_get_perfdata, NULL, "nebmodule.perfdata", NULL },
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

static PyObject *NebHostCheck_get_host (NebHostCheck *self)
{
    if (self->host)
    {
        Py_INCREF (self->host);
    }

    return self->host;
}

static PyObject *NebHostCheck_get_hostname (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->host_name);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_current_attempt (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->current_attempt);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_max_attempts (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->max_attempts);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_check_type (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->check_type);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_state_type (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->state_type);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_state (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->state);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_command_name (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->command_name);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_command_args (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->command_args);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_command_line (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->command_line);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_start_time (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->start_time.tv_sec);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_end_time (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->end_time.tv_sec);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_exec_time (NebHostCheck *self)
{
    if (self->data)
    {
        return PyFloat_FromDouble (self->data->execution_time);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_latency (NebHostCheck *self)
{
    if (self->data)
    {
        return PyFloat_FromDouble (self->data->latency);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_timeout (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->timeout);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_early_timeout (NebHostCheck *self)
{
    if (self->data)
    {
        return self->data->early_timeout ? Py_True : Py_False;
    }

    return NULL;
}

static PyObject *NebHostCheck_get_return_code (NebHostCheck *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->return_code);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_output (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->output);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_long_output (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->long_output);
    }

    return NULL;
}

static PyObject *NebHostCheck_get_perfdata (NebHostCheck *self)
{
    if (self->data)
    {
        return PyString_FromString (self->data->perf_data);
    }

    return NULL;
}

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
