/*
 * Linking module for PuTTY proper: list the available backends
 * including ssh, plus the serial backend.
 */

#include <stdio.h>
#include "putty.h"

/*
 * This appname is not strictly in the right place, since Plink
 * also uses this module. However, Plink doesn't currently use any
 * of the dialog-box sorts of things that make use of appname, so
 * it shouldn't do any harm here. I'm trying to avoid having to
 * have tiny little source modules containing nothing but
 * declarations of appname, for as long as I can...
 */
#ifdef PUTTYR4S
const char *const appname = "PuTTY R4S";
#else
const char *const appname = "PuTTY";
#endif // PUTTYR4S


#ifdef TELNET_DEFAULT
const int be_default_protocol = PROT_TELNET;
#else
const int be_default_protocol = PROT_SSH;
#endif

Backend *backends[] = {
    &ssh_backend,
    &telnet_backend,
    &rlogin_backend,
    &raw_backend,
    &serial_backend,
    NULL
};