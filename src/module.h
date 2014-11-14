#ifndef __MODULE_H
#define __MODULE_H

#include <nagios/nebmodules.h>
#include <nagios/nebcallbacks.h>

int nebmodule_init (
    int flags __attribute__ ((__unused__)),
    char *args,
    nebmodule *handle
);

int nebmodule_deinit (
    int flags __attribute__ ((__unused__)),
    int reason __attribute__ ((__unused__))
);

#endif /* __MODULE_H */