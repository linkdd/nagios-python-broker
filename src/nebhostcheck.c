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

#include "nebprocess.h"
#include "nebhostcheck.h"
#include "nagioshost.h"
#include <structmember.h>

struct _NebHostCheck
{
    NebProcess parent;

    nebstruct_host_check_data *hostcheck;
    PyObject *host;
};

/* methods */
static void NebHostCheck_dealloc (NebHostCheck *self);
static PyObject *NebHostCheck_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NebHostCheck_init (NebHostCheck *self, PyObject *args, PyObject *kwargs);

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
    { NULL }
};

static PyTypeObject _NebHostCheckType =
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
    (PyObject *) (NebProcessType),
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) NebHostCheck_init,
    0,                                  /* tp_alloc */
    NebHostCheck_new
};

PyTypeObject *NebHostCheckType = &_NebHostCheckType

/* implementation */

void NebHostCheckType_Initialize (PyObject *namespace)
{
    NebProcessType_Initialize (namespace);

    if (PyType_Ready (NebHostCheckType) < 0)
    {
        return;
    }

    Py_INCREF (NebHostCheckType);
    PyModule_AddObject (namespace, "NebHostCheck", (PyObject *) (NebHostCheckType));
}

static void NebHostCheck_dealloc (NebHostCheck *self)
{
    Py_XDECREF (self->host);
    NebProcessType->tp_dealloc (self);
}

static PyObject *NebHostCheck_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    NebHostCheck *self = (NebHostCheck *) NebProcessType->tp_new (type, args, kwargs);

    if (self != NULL)
    {
        self->hostcheck = NULL;
        self->host = NULL;
    }

    return (PyObject *) self;
}

static int NebHostCheck_init (NebHostCheck *self, PyObject *args, PyObject *kwargs)
{
    int ret = NebProcessType->tp_init (self, args, kwargs));

    if (ret == 0)
    {
        self->hostcheck = (nebstruct_host_check_data *) self->parent.data;
        self->host = NagiosHost_New (self->hostcheck->object_ptr);
    }

    return ret;
}
