#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

.text
LEAF(_my_getchar)
	li t0, 0xb0000000
	lb v0, 0(t0)
	sb v0, 0(t0)
	j ra



END(_my_getchar)

LEAF(_my_exit)
	li t0, 0xb0000010
	lb t1, 0(t0)
END(_my_exit)
