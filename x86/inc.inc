;;; ------------------------------------------------------------
;;; /usr/include/asm/unistd_32.h
define exit         1
define fork         2
define read         3
define write        4
define open         5
define close        6
define waitpid      7
define unlink      10
define execve      11
define chdir       12
define mknod       14
define getpid      20
define mount       21
define setuid      23
define sync        36
define kill        37
define mkdir       39
define signal      48
define umount      52
define dup2        63
define setsid      66
define sigaction   67
define sethostname 74
define symlink     83
define reboot      88

;;; ------------------------------------------------------------
;;; /usr/include/bits/fcntl-linux.h
define O_RDONLY       0
define O_WRONLY       1
define O_RDWR         2
define O_CREAT       64
define O_APPEND    1024
define O_CLOEXEC 524288

;;; ------------------------------------------------------------
;;; /usr/include/bits/signum-generic.h
define SIGINT   2
define SIGKILL  9
define SIGTERM 15
define SIGCHLD 20

;; vim: set ft=fasm:
