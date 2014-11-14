#ifndef __PYCLASS_NAGIOS_HOST_H
#define __PYCLASS_NAGIOS_HOST_H

#include <nagios/objects.h>
#include <Python.h>

typedef struct _NagiosHost NagiosHost;

void NagiosHostType_Initialize (PyObject *namespace);
PyObject *NagiosHost_New (host *data);

PyObject *NagiosHost_AddParent (PyObject *self, PyObject *hostname);
PyObject *NagiosHost_AddChild (PyObject *self, PyObject *childhost);
PyObject *NagiosHost_AddContactGroup (PyObject *self, PyObject *cgroupname);
PyObject *NagiosHost_AddContact (PyObject *self, PyObject *contactname);
PyObject *NagiosHost_AddCustomVariable (PyObject *self, PyObject *varname, PyObject *value);
PyObject *NagiosHost_AddServiceLink (PyObject *self, PyObject *servicelink);

#endif /* __PYCLASS_NAGIOS_HOST_H */