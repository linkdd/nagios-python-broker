#ifndef __PYCLASS_NEB_HOSTCHECK_H
#define __PYCLASS_NEB_HOSTCHECK_H

#include <nagios/nebstructs.h>
#include <Python.h>

extern PyTypeObject *NebHostCheckType;
typedef struct _NebHostCheck NebHostCheck;

void NebHostCheckType_Initialize (PyObject *namespace);
PyObject *NebHostCheck_New (nebstruct_host_check_data *data);

#endif /* __PYCLASS_NEB_HOSTCHECK_H */