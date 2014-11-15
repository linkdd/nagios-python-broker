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

static int nebmodule_main (NebModule *handle)
{
    nebmodule_register (handle);

    return 0;
}

int nebmodule_init (
    int flags __attribute__ ((__unused__)),
    char *args,
    nebmodule *handle)
{
    PyObject *module = NULL;

    Py_SetProgramName ("nagios-python-broker");
    Py_Initialize ();

    NebModuleType_Initialize (module);

    return nebmodule_main (module);
}

int nebmodule_deinit (
    int flags __attribute__ ((__unused__)),
    int reason __attribute__ ((__unused__)))
{
    nebmodule_deregister ();

    Py_Finalize ();
}
