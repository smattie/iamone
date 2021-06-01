// ----------------------------------------------------------------------------
// shutdown
// copyright 2020 smattie
//
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING file for more details.
// ----------------------------------------------------------------------------

#define  _POSIX_C_SOURCE 200112L
#include <signal.h>
#include <unistd.h>

extern int
main (int ac, char *av[]) {
	if (kill (1, SIGTERM) != 0) {
		write (2, "error sending SIGTERM to init\n", 30);
		return 1; }

	return 0; }
