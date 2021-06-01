;; ----------------------------------------------------------------------------
;; reboot
;; copyright 2020 smattie
;;
;; This work is free. You can redistribute it and/or modify it under the
;; terms of the Do What The Fuck You Want To Public License, Version 2,
;; as published by Sam Hocevar. See the COPYING file for more details.
;; ----------------------------------------------------------------------------

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
	mov    cl, SIGINT
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
	msg db "error sending SIGINT to init", 0x0a
	.len = $ - msg

;; vim: set ft=fasm:
