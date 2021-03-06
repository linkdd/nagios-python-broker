#ifndef __PYCLASS_MACROS_H
#define __PYCLASS_MACROS_H

#include <Python.h>

#define PYCLASS_DECL_GETPROP(type,name)                 \
    PyObject* _##type##_get_##name (type *self)     \

#define PYCLASS_DECL_SETPROP(type,name)                                \
    void _##type##_set_##name (type *self, PyObject *obj)     \

#define PYCLASS_DEF_GETPROP(type,name,ptr,transform)    \
    PyObject* _##type##_get_##name (type *self)     \
    {                                                   \
        return transform (self->ptr);                   \
    }

#define PYCLASS_DEF_GETPROP_WITH_CODE(type,name)        \
    PyObject* _##type##_get_##name (type *self)


#define PYCLASS_DEF_SETPROP(type,name,ptr,transform)            \
    void _##type##_set_##name (type *self, PyObject *obj)   \
    {                                                           \
        self->ptr = transform (obj);                            \
    }

#define PYCLASS_DEF_SETPROP_WITH_CODE(type,name)                \
    void _##type##_set_##name (type *self, PyObject *obj)

#define PYCLASS_ADD_PROP(name,getfunc,setfunc,doc)      \
    {                                                   \
        name,                                           \
        (getter) getfunc,                               \
        (setter) setfunc,                               \
        doc,                                            \
        NULL                                            \
    }

#define PYCLASS_GETPROP(type,name)  _##type##_get_##name
#define PYCLASS_SETPROP(type,name)  _##type##_set_##name

static inline PyObject *Py_INCREFRET (PyObject *o)
{
    Py_XINCREF (o);
    return o;
}

#endif /* __PYCLASS_MACROS_H */