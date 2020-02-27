/// ----------------------------------------------------------------
/// init
/// https://github.com/smattie/iamone
///
/// copyright 2020 smattie
/// licensed under the GNU GPL v3
/// ----------------------------------------------------------------

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

static int action;

static char *rcargv[] = {
	"/bin/initrc", 0, 0 };

static void
sighand (int n) {
	action = n == SIGINT ? RB_AUTOBOOT : RB_POWER_OFF;
	signal (SIGINT,  SIG_DFL);
	signal (SIGTERM, SIG_DFL);
	return; }

extern int
main (int ac, char *av[], char *ev[]) {
	if (getpid () != 1) {
		return -1; }

	mkdir ("/dev/pts", 00755);
	mkdir ("/dev/shm", 01777);

	mount ("devpts", "/dev/pts", "devpts", MS_NOSUID | MS_NOEXEC,            "gid=5,mode=0620");
	mount ("proc",   "/proc",    "proc",   MS_NOSUID | MS_NOEXEC,            0);
	mount ("sysfs",  "/sys",     "sysfs",  MS_NOSUID | MS_NOEXEC,            0);
	mount ("tmpfs",  "/dev/shm", "tmpfs",  MS_NOSUID | MS_NOEXEC | MS_NODEV, "mode=1777");
	mount ("tmpfs",  "/run",     "tmpfs",  MS_NOSUID | MS_NOEXEC | MS_NODEV, "mode=0755,size=10%");
	mount ("tmpfs",  "/tmp",     "tmpfs",  MS_NOSUID | MS_NODEV,             "size=50%");

	symlink ("/proc/self/fd",   "/dev/fd");
	symlink ("/proc/self/fd/0", "/dev/stdin");
	symlink ("/proc/self/fd/1", "/dev/stdout");
	symlink ("/proc/self/fd/2", "/dev/stderr");

	reboot (RB_DISABLE_CAD);

	struct sigaction sigsetup = { .sa_handler = &sighand };
	sigaction (SIGINT,  &sigsetup, 0);
	sigaction (SIGTERM, &sigsetup, 0);

	if (fork () == 0) {
		execve (rcargv[0], rcargv, ev);
		exit   (1); }

	for (;;) {
		waitpid (-1, 0, 0);
		if (action) {
			break; }}

	rcargv[1] = "-s";

	pid_t rc = fork ();
	if (rc == 0) {
		execve (rcargv[0], rcargv, ev);
		exit   (1); }

	waitpid (rc, 0, 0);
	reboot  (action);

	return 0; }
