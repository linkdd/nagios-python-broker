#include "module.h"
#include <Python.h>

static int nebmodule_callback (int event_type, void *data)
{
    return 0;
}

static void nebmodule_register (nebmodule *handle)
{
    int nebcallback;

    /* NEB callbacks are identified as int by macros, 0 to 4 is reserved */
    for (nebcallback = 5 ; nebcallback < NEBCALLBACK_NUMITEMS ; ++nebcallback)
    {
        (void) neb_register_callback (nebcallback, handle, 0, nebmodule_callback);
    }
}

static void nebmodule_deregister (void)
{
    int nebcallback;

    /* NEB callbacks are identified as int by macros, 0 to 4 is reserved */
    for (nebcallback = 5 ; nebcallback < NEBCALLBACK_NUMITEMS ; ++nebcallback)
    {
        (void) neb_deregister_callback (nebcallback, nebmodule_callback);
    }
}

static int nebmodule_main (nebmodule *handle, char *package)
{
    nebmodule_register (handle);

    return 0;
}

int nebmodule_init (
    int flags __attribute__ ((__unused__)),
    char *args,
    nebmodule *handle)
{
    Py_SetProgramName ("nagios-python-broker");
    Py_Initialize ();

    return nebmodule_main (handle, args);
}

int nebmodule_deinit (
    int flags __attribute__ ((__unused__)),
    int reason __attribute__ ((__unused__)))
{
    nebmodule_deregister ();

    Py_Finalize ();
}
