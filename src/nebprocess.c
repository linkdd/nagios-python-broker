#include "nebprocess.h"
#include <structmember.h>

struct _NebProcess
{
    PyObject_HEAD

    nebstruct_process_data *data;
};

/* methods */
static void NebProcess_dealloc (NebProcess *self);
static PyObject *NebProcess_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
static int NebProcess_init (NebProcess *self, PyObject *args, PyObject *kwargs);

/* properties */
static PyObject *NebProcess_get_type (NebProcess *self);
static PyObject *NebProcess_get_flags (NebProcess *self);
static PyObject *NebProcess_get_attr (NebProcess *self);
static PyObject *NebProcess_get_timestamp (NebProcess *self);

/* vtables */
static PyMethodDef NebProcess_methods[] =
{
    { NULL }
};

static PyMemberDef NebProcess_members[] =
{
    { NULL }
};

static PyGetSetDef NebProcess_getseters[] =
{
    {"type",      (getter) NebProcess_get_type,      NULL, "nebstruct_process_data.type",      NULL},
    {"flags",     (getter) NebProcess_get_flags,     NULL, "nebstruct_process_data.flags",     NULL},
    {"attr",      (getter) NebProcess_get_attr,      NULL, "nebstruct_process_data.attr",      NULL},
    {"timestamp", (getter) NebProcess_get_timestamp, NULL, "nebstruct_process_data.timestamp", NULL},
    { NULL }
};

static PyTypeObject NebProcessType =
{
    PyObject_HEAD_INIT (NULL)
    0,
    "NebProcess",
    sizeof (NebProcess),
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
    "nebstruct_process_data wrapper",
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NebProcess_methods,
    NebProcess_members,
    NebProcess_getseters,
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) NebProcess_init,
    0,                                  /* tp_alloc */
    NebProcess_new
};

/* implementation */

void NebProcessType_Initialize (PyObject *namespace)
{
    if (PyType_Ready (&NebProcessType) < 0)
    {
        return;
    }

    Py_INCREF (&NebProcessType);
    PyModule_AddObject (namespace, "NebProcess", (PyObject *) (&NebProcessType));
}

static void NebProcess_dealloc (NebProcess *self)
{
    self->ob_type->tp_free ((PyObject *) self);
}

static PyObject *NebProcess_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    NebProcess *self = (NebProcess *) type->tp_alloc (type, 0);

    if (self != NULL)
    {
        self->data = NULL;
    }

    return (PyObject *) self;
}

static int NebProcess_init (NebProcess *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords (args, kwargs, "|O&", kwlist, &(self->data)))
    {
        return -1;
    }

    return 0;
}

static PyObject *NebProcess_get_type (NebProcess *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->type);
    }

    return NULL;
}

static PyObject *NebProcess_get_flags (NebProcess *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->flags);
    }

    return NULL;
}

static PyObject *NebProcess_get_attr (NebProcess *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->attr);
    }

    return NULL;
}

static PyObject *NebProcess_get_timestamp (NebProcess *self)
{
    if (self->data)
    {
        return PyInt_FromLong (self->data->timestamp.tv_sec);
    }

    return NULL;
}

PyObject *NebProcess_New (nebstruct_process_data *data)
{
    PyObject *nebprocess = Py_BuildValue ("0&", data);

    return PyObject_CallObject (
        (PyObject *) (&NebProcessType),
        nebprocess
    );
}
