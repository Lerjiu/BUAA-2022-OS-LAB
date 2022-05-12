#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>
/*
void mips_init()
{
	u_int *target;
	target = 0x80000080;
	printf("before mips_detect_memory: 0x%x\n",*target);
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();
	printf("before mips_vm_init: 0x%x\n",*target);
	mips_vm_init();
	printf("before page_init: 0x%x\n",*target);
	page_init();
	printf("before env_init: 0x%x\n",*target);
	env_init();
	printf("before env_check: 0x%x\n",*target);
	env_check();
	printf("before load_icode_check: 0x%x\n",*target);
	load_icode_check();

	/*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
	 * code_b.c*/
	/*** exercise 3.9 ***/
	/*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
	 * interesting, have fun please*/

/*	printf("before create A: 0x%x\n",*target);
	ENV_CREATE_PRIORITY(user_A, 2);
	printf("before create B: 0x%x\n",*target);
	ENV_CREATE_PRIORITY(user_B, 1);
	printf("before trap_init: 0x%x\n",*target);
	trap_init();
	printf("before kclock_init: 0x%x\n",*target);
	kclock_init();
	printf("before panic\n");
	panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
	printf("after panic\n");
	while(1);
	panic("init.c:\tend of mips_init() reached!");
}*/

void mips_init()
{
printf("init.c:\tmips_init() is called\n");
mips_detect_memory();
mips_vm_init();
page_init();
env_init();
// for lab3-2-exam local test
ENV_CREATE_PRIORITY(user_A, 2);
ENV_CREATE_PRIORITY(user_B, 1);
trap_init();
kclock_init();
panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
while(1);
panic("init.c:\tend of mips_init() reached!");
}


void bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	max = dst + len;
	// copy machine words while possible
	while (dst + 3 < max)
	{
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}
	// finish remaining 0-3 bytes
	while (dst < max)
	{
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len)
{
	void *max;

	max = b + len;

	//printf("init.c:\tzero from %x to %x\n",(int)b,(int)max);

	// zero machine words while possible

	while (b + 3 < max)
	{
		*(int *)b = 0;
		b+=4;
	}

	// finish remaining 0-3 bytes
	while (b < max)
	{
		*(char *)b++ = 0;
	}

}
