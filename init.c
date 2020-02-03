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

static pid_t rc;
static int doPoweroff;
static int doRestart;

static char *envp[] = {
	"HOME=/",
	"TERM=linux",
	0 };

static void
restart (int n) {
	kill (rc, SIGTERM);
	doRestart = 1;
	return; }

static void
poweroff (int n) {
	kill (rc, SIGTERM);
	doPoweroff = 1;
	return; }

extern int
main (int ac, char *av[]) {
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

	signal (SIGINT,  restart);
	signal (SIGTERM, poweroff);

	rc = fork ();
	if (rc == 0) {
		execle ("/bin/rc", "rc", (char *)0, envp);
		exit   (1); }

	for (;;) {
		pid_t dead = waitpid (-1, 0, 0);
		if (rc == dead) {
			if (doPoweroff != 0) { reboot (RB_POWER_OFF); }
			if (doRestart  != 0) { reboot (RB_AUTOBOOT);  }}}

	return 0; }
