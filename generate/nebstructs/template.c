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

#include "{{source.file}}.h"
#include <pyclassmacros.h>
#include <structmember.h>
#include <time.h>

/* methods */
void _{{class.name}}_dealloc ({{class.name}} *self);
PyObject *_{{class.name}}_new (PyTypeObject *type, PyObject *args, PyObject *kwargs);
int _{{class.name}}_init ({{class.name}} *self, PyObject *args, PyObject *kwargs);

/* properties */
{% for prop in class.properties %}
    PYCLASS_DECL_GETPROP ({{class.name}}, {{prop.name}});
    PYCLASS_DECL_SETPROP ({{class.name}}, {{prop.name}});
{% endfor %}

/* vtables */
static PyMethodDef {{class.name}}_methods[] =
{
    { NULL }
};

static PyMemberDef {{class.name}}_members[] =
{
    { NULL }
};

static PyGetSetDef {{class.name}}_getseters[] =
{
    {% for prop in class.properties %}
        PYCLASS_ADD_PROP(
            "{{prop.name}}",
            PYCLASS_GETPROP ({{class.name}}, {{prop.name}}),
            PYCLASS_SETPROP ({{class.name}}, {{prop.name}}),
            "{{class.name}}.{{prop.name}}"
        ),
    {% endfor %}

    { NULL }
};

PyTypeObject {{class.name}}Type =
{
    PyObject_HEAD_INIT (NULL)
    0,
    "{{class.name}}",
    sizeof ({{class.name}}),
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
    "{{struct.name}} wrapper",
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    {{class.name}}_methods,
    {{class.name}}_members,
    {{class.name}}_getseters,
    (PyTypeObject *) (&{{class.name}}Type),
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc) _{{class.name}}_init,
    0,                                  /* tp_alloc */
    _{{class.name}}_new
};

/* implementation */

void {{class.name}}Type_Initialize (PyObject *namespace)
{
    {{class.name}}Type_Initialize (namespace);

    if (PyType_Ready (&{{class.name}}Type) < 0)
    {
        return;
    }

    Py_INCREF (&{{class.name}}Type);
    PyModule_AddObject (namespace, "{{class.name}}", (PyObject *) (&{{class.name}}Type));
}

void _{{class.name}}_dealloc ({{class.name}} *self)
{
    {{class.name}}Type.tp_dealloc ((PyObject *) self);
}

PyObject *_{{class.name}}_new (PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    {{class.name}} *self = ({{class.name}} *) {{class.name}}Type.tp_new (type, args, kwargs);

    if (self != NULL)
    {
        self->data = NULL;
    }

    return (PyObject *) self;
}

int _{{class.name}}_init ({{class.name}} *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = { NULL };

    if (!PyArg_ParseTupleAndKeywords (args, kwargs, "|O&", kwlist, &(self->data)))
    {
        return -1;
    }

    return 0;
}

{% for prop in class.properties %}
    {% if prop.with_code %}
        PYCLASS_DEF_GETPROP_WITH_CODE ({{class.name}}, {{prop.name}})
        {
            {{prop.getcode}}
        }

        PYCLASS_DEF_SETPROP_WITH_CODE ({{class.name}}, {{prop.name}})
        {
            {{prop.setcode}}
        }
    {% else %}
        PYCLASS_DEF_GETPROP (
            {{class.name}},
            {{prop.name}},
            data->{{prop.name}},
            {{prop.getcode}}
        )
        PYCLASS_DEF_SETPROP (
            {{class.name}},
            {{prop.name}},
            data->{{prop.name}},
            {{prop.setcode}}
        )
    {% endif %}
{% endfor %}

PyObject *{{class.name}}_New ({{struct.name}} *data)
{
    PyObject *arguments = Py_BuildValue ("(0&)", data);

    PyObject *self = PyObject_CallObject (
        (PyObject *) (&{{class.name}}Type),
        arguments
    );

    Py_DECREF (arguments);

    return self;
}
