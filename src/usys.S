#include "syscall.h"

# user syscall entrypoint.

.global gethid
gethid:
	li a7, SYS_gethid
	ecall
	ret
