	.section .text
	.proc 
_start::
	c0	mov $r0.1 = 0x1000
;;
	c0	call $l0.0 = main
;;
	c0	stop 
;;
	c0	nop
;;
	.endp
