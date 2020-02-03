/// ----------------------------------------------------------------
/// reboot
/// https://github.com/smattie/iamone
///
/// copyright 2020 smattie
/// licensed under the GNU GPL v3
/// ----------------------------------------------------------------

#include <signal.h>
#include <unistd.h>

extern int
main (int ac, char *av[]) {
	if (kill (1, SIGINT) != 0) {
		write (2, "error sending SIGINT to init\n", 29);
		return 1; }

	return 0; }
