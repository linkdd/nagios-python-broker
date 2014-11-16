#include "logging.h"

#include <nagios/nagios.h>
#include <Python.h>
#include <stdarg.h>

static void nebmodule_log (int level, const char *format, va_list args)
{
    PyObject *string = PyString_FromFormatV (format, args);

    write_to_all_logs (PyString_AsString (string), level);

    Py_DECREF (string);
}

void nebmodule_log_debug (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    nebmodule_log (1, format, args);
    va_end (args);
}

void nebmodule_log_info (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    nebmodule_log (NSLOG_INFO_MESSAGE, format, args);
    va_end (args);
}

void nebmodule_log_warn (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    nebmodule_log (NSLOG_RUNTIME_WARNING, format, args);
    va_end (args);
}

void nebmodule_log_error (const char *format, ...)
{
    va_list args;

    va_start (args, format);
    nebmodule_log (NSLOG_RUNTIME_ERROR, format, args);
    va_end (args);
}

void nebmodule_log_exception (void)
{
    PyObject *errtype = NULL, *errtypestr = NULL;
    PyObject *errobj = NULL, *errobjstr = NULL;
    PyObject *traceback = NULL, *tbstr = NULL;

    PyErr_Fetch (&errtype, &errobj, &traceback);

    errtypestr = PyObject_Str (errtype);
    Py_DECREF (errtype);

    errobjstr = PyObject_Str (errobj);
    Py_DECREF (errobj);

    tbstr = PyObject_Str (traceback);
    Py_DECREF (traceback);

    nebmodule_log_error (
        "Error %s: %s",
        PyString_AsString (errtypestr),
        PyString_AsString (errobjstr)
    );

    nebmodule_log_debug (
        "Traceback: %s",
        PyString_AsString (tbstr)
    );

    Py_DECREF (errtypestr);
    Py_DECREF (errobjstr);
    Py_DECREF (tbstr);
}
