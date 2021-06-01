;; ----------------------------------------------------------------------------
;; init
;; copyright 2020 smattie
;;
;; This work is free. You can redistribute it and/or modify it under the
;; terms of the Do What The Fuck You Want To Public License, Version 2,
;; as published by Sam Hocevar. See the COPYING file for more details.
;; ----------------------------------------------------------------------------

format ELF executable 3
entry start

include "inc.inc"

define LINUX_REBOOT_MAGIC1  0xfee1dead
define LINUX_REBOOT_MAGIC2C 0x20112000

define LINUX_REBOOT_CMD_CAD_OFF   0
define LINUX_REBOOT_CMD_RESTART   0x01234567
define LINUX_REBOOT_CMD_POWER_OFF 0x4321fedc

segment readable writable executable
start:

	mov   ecx, dword [esp]
	lea   esp, [esp + ecx*4 + 8]

	xor   eax, eax
	xor   edx, edx
	xor   esi, esi
	xor   edi, edi
	xor   ebp, ebp

	mov    al, getpid
	int   80h
	dec   eax
	jnz   .exit

	mov    bp, mount
	mov   eax, ebp
	mov   ebx, devfs
	mov   ecx, dev
	mov   edx, ebx
	int   80h

	mov   edx, mkdir
	mov   eax, edx
	mov   ebx, devpts
	mov   ecx, 0755o
	int   80h

	mov   eax, edx
	mov   ebx, devshm
	mov   ecx, 1777o
	int   80h

	mov   eax, ebp
	mov   ebx, procfs
	mov   ecx, proc
	mov   edx, ebx
	int   80h

	mov   eax, ebp
	mov   ebx, sysfs
	mov   ecx, sys
	mov   edx, ebx
	int   80h

	mov   eax, ebp
	mov   ebx, ptsfs
	mov   ecx, devpts
	mov   edx, ebx
	mov   edi, ptsopt
	int   80h

	mov   eax, ebp
	mov   ebx, devfs + 3
	mov   ecx, devshm
	mov   edx, ebx
	mov   edi, shmopt
	int   80h

	mov    bp, symlink
	mov   eax, ebp
	mov   ebx, procfd
	mov   ecx, devfd
	int   80h

	mov   eax, ebp
	mov   ebx, procin
	mov   ecx, devin
	int   80h

	mov   eax, ebp
	mov   ebx, procout
	mov   ecx, devout
	int   80h

	mov   eax, ebp
	mov   ebx, procerr
	mov   ecx, deverr
	int   80h

	mov    al, reboot
	mov   ebx, LINUX_REBOOT_MAGIC1
	mov   ecx, LINUX_REBOOT_MAGIC2C
	xor   edx, edx
	xor   esi, esi
	int   80h

	mov   [sigsetup], dword sighand
	mov    al, sigaction
	mov   ebx, SIGINT
	mov   ecx, sigsetup
	int   80h

	mov    al, sigaction
	mov    bl, SIGTERM
	int   80h

	xor   ebp, ebp      ;;; whether or not we'll call reboot
	mov   [action], ebp ;;; and what will happen if we do

.forkrc:
	mov    al, fork
	xor   ebx, ebx
	int   80h
	test  eax, eax
	jz    .execrc

	or    ebx, -1
	xor   ecx, ecx

	test   ebp, ebp
	cmovnz ebx, eax

.bringoutyourdead:
	mov   eax, waitpid
	int   80h
	mov   edi, [action]
	test  edi, edi
	jz    .bringoutyourdead
	test  ebp, ebp
	jnz   .reboot

	mov   [rcargv + 4], rcflag
	xor   eax, eax
	or    ebp, -1
	jmp   .forkrc

.reboot:
	mov   eax, reboot
	mov   ebx, LINUX_REBOOT_MAGIC1
	mov   ecx, LINUX_REBOOT_MAGIC2C
	mov   edx, edi
	int   80h
	jmp   .bringoutyourdead

.execrc:
	mov    al, execve
	mov   ebx, rcpath
	mov   ecx, rcargv
	mov   edx, esp
	int   80h

.exit:
	xor   eax, eax
	inc   eax
	int   80h

;;; ------------------------------------------------------------

sighand:
	mov   ebx, LINUX_REBOOT_CMD_POWER_OFF
	mov   ecx, LINUX_REBOOT_CMD_RESTART
	cmp   eax, SIGINT
	cmove ebx, ecx
	mov   [action], ebx

	mov    al, signal
	mov   ebx, SIGINT
	xor   ecx, ecx
	int   80h
	mov    al, signal
	mov    bl, SIGTERM
	int   80h
	ret

;;; ------------------------------------------------------------

align 4
	rcargv dd rcbin, 0, 0

	rcpath db "/bin/"
	rcbin  db "initrc", 0
	rcflag db "-s", 0

	devfs  db "devtmpfs", 0
	ptsfs  db "devpts", 0
	procfs db "proc", 0
	sysfs  db "sysfs", 0

	ptsopt db "mode=0620,gid=5", 0
	shmopt db "mode=1777", 0

	dev    db "/dev", 0
	devpts db "/dev/pts", 0
	devshm db "/dev/shm", 0
	proc   db "/proc", 0
	sys    db "/sys", 0

	procfd  db "/proc/self/fd", 0
	procin  db "/proc/self/fd/0", 0
	procout db "/proc/self/fd/1", 0
	procerr db "/proc/self/fd/2", 0

	devfd  db "/dev/fd", 0
	devin  db "/dev/stdin", 0
	devout db "/dev/stdout", 0
	deverr db "/dev/stderr", 0

action:
sigsetup:
	rb 156

;; vim: set ft=fasm:
