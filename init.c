// ----------------------------------------------------------------------------
// init
// copyright 2020 smattie
//
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING file for more details.
// ----------------------------------------------------------------------------

#define  _POSIX_C_SOURCE 200112L
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
	struct sigaction sigsetup = { .sa_handler = &sighand };
	pid_t rc;

	if (getpid () != 1) {
		return -1; }

	mount ("devtmpfs", "/dev", "devtmpfs", 0, 0);

	mkdir ("/dev/pts", 00755);
	mkdir ("/dev/shm", 01777);

	mount ("proc",   "/proc",    "proc",   0, 0);
	mount ("sysfs",  "/sys",     "sysfs",  0, 0);
	mount ("devpts", "/dev/pts", "devpts", 0, "gid=5,mode=0620");
	mount ("tmpfs",  "/dev/shm", "tmpfs",  0, "mode=1777");

	symlink ("/proc/self/fd",   "/dev/fd");
	symlink ("/proc/self/fd/0", "/dev/stdin");
	symlink ("/proc/self/fd/1", "/dev/stdout");
	symlink ("/proc/self/fd/2", "/dev/stderr");

	reboot (RB_DISABLE_CAD);

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

	rc = fork ();
	if (rc == 0) {
		execve (rcargv[0], rcargv, ev);
		exit   (1); }

	waitpid (rc, 0, 0);
	reboot  (action);

	return 0; }
