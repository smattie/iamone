/// ----------------------------------------------------------------
/// shutdown
/// https://github.com/smattie/iamone
///
/// copyright 2020 smattie
/// licensed under the GNU GPL v3
/// ----------------------------------------------------------------

#include <signal.h>
#include <unistd.h>

extern int
main (int ac, char *av[]) {
	if (kill (1, SIGTERM) != 0) {
		write (2, "error sending SIGTERM to init\n", 30);
		return 1; }

	return 0; }
