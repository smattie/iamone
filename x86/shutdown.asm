;;; ----------------------------------------------------------------
;;; shutdown
;;; https://github.com/smattie/iamone
;;;
;;; copyright 2020 smattie
;;; licensed under the GNU GPL v3
;;; ----------------------------------------------------------------

format ELF executable 3
entry start

include "inc.inc"

segment readable executable
start:

	xor   eax, eax
	xor   ebx, ebx
	xor   ecx, ecx
	xor   edx, edx

	mov    al, kill
	inc   ebx
	mov    cl, SIGTERM
	int   80h
	test  eax, eax
	jz    .exit

	mov    al, write
	inc   ebx
	mov   ecx, msg
	mov    dl, msg.len
	int   80h

.exit:
	xor   eax, eax
	inc   eax
	dec   ebx
	int   80h

align 1
	msg db "error sending SIGTERM to init", 0x0a
	.len = $ - msg

;; vim: set ft=fasm:
