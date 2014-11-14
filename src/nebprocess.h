#ifndef __PYCLASS_NEB_PROCESS_H
#define __PYCLASS_NEB_PROCESS_H

#include <nagios/nebstructs.h>
#include <Python.h>

typedef struct _NebProcess NebProcess;

void NebProcessType_Initialize (PyObject *namespace);
PyObject *NebProcess_New (nebstruct_process_data *data);

#endif /* __PYCLASS_NEB_PROCESS_H */