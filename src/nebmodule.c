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

#include "nebmodule.h"
#include <structmember.h>

struct _NebModule
{
    PyObject_HEAD

    nebmodule *handle;
};

/* methods */
static void NebModule_dealloc (NebModule *self);
static PyObject *NebModule_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NebModule_init (NebModule *self, PyObject *args, PyObject *kwargs);

static PyObject *NebModule_should_be_loaded (NebModule *self);
static PyObject *NebModule_is_currently_loaded (NebModule *self);
static PyObject *NebModule_call (NebModule *self, PyObject *evtype);

/* properties */
static PyObject *NebModule_get_filename (NebModule *self);
static PyObject *NebModule_get_args (NebModule *self);
static PyObject *NebModule_get_threadid (NebModule *self);

/* vtables */
static PyMethodDef NebModule_methods[] =
{
    { "should_be_loaded", (PyCFunction) NebModule_should_be_loaded, METH_NOARGS, NULL },
    { "is_currently_loaded", (PyCFunction) NebModule_is_currently_loaded, METH_NOARGS, NULL },
    { "__call__", (PyCFunction) NebModule_call, METH_O, NULL },
    { NULL }
};

static PyMemberDef NebModule_members[] =
{
    { NULL }
};

static PyGetSetDef NebModule_getseters[] =
{
    { "filename", (getter) NebModule_get_filename, NULL, "nebmodule.filename", NULL },
    { "args",     (getter) NebModule_get_args,     NULL, "nebmodule.args",     NULL },
    { "threadid", (getter) NebModule_get_threadid, NULL, "nebmodule.threadid", NULL },
    { NULL }
};

static PyTypeObject _NebModuleType =
{
    PyObject_HEAD_INIT (NULL)
    0,
    "NebModule",
    sizeof (NebModule),
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
    "nebmodule wrapper",
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NebModule_methods,
    NebModule_members,
    NebModule_getseters,
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) NebModule_init,
    0,                                  /* tp_alloc */
    NebModule_new
};

PyTypeObject *NebModuleType = &_NebModuleType;

/* implementation */

void NebModuleType_Initialize (PyObject *namespace)
{
    if (PyType_Ready (NebModuleType) < 0)
    {
        return;
    }

    Py_INCREF (NebModuleType);
    PyModule_AddObject (namespace, "NebModule", (PyObject *) (NebModuleType));
}

static void NebModule_dealloc (NebModule *self)
{
    self->ob_type->tp_free ((PyObject *) self);
}

static PyObject *NebModule_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    NebModule *self = (NebModule *) type->tp_alloc (type, 0);

    if (self != NULL)
    {
        self->handle = NULL;
    }

    return (PyObject *) self;
}

static int NebModule_init (NebModule *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords (args, kwargs, "|O&", kwlist, &(self->handle)))
    {
        return -1;
    }

    return 0;
}

static PyObject *NebModule_get_filename (NebModule *self)
{
    if (self->handle)
    {
        return PyString_FromString (self->handle->filename);
    }

    return NULL;
}

static PyObject *NebModule_get_args (NebModule *self)
{
    if (self->handle)
    {
        return PyString_FromString (self->handle->args);
    }

    return NULL;
}

static PyObject *NebModule_threadid (NebModule *self)
{
    if (self->handle)
    {
#ifdef HAVE_PTHREAD_H
        int tid = self->handle->thread_id;
#else
        int tid = -1;
#endif

        return PyInt_FromLong (tid);
    }

    return NULL;
}

static PyObject *NebModule_should_be_loaded (NebModule *self)
{
    return (self->handle && self->handle->should_be_loaded ? Py_True : Py_False);
}

static PyObject *NebModule_is_currently_loaded (NebModule *self)
{
    return (self->handle && self->handle->is_currently_loaded ? Py_True : Py_False);
}

static PyObject *NebModule_call (NebModule *self, PyObject *evtype)
{
    return PyInt_FromLong (0);
}

nebmodule *NebModule_GetHandle (NebModule *self)
{
    return self->handle;
}