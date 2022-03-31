/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

struct my_struct{
	int size;
	char ch;
	int arr[3];
};
int main()
{
	struct my_struct a;
	a.size = 3;
	a.ch = 'g';
	a.arr[0] = -1;
	a.arr[1] = 11;
	a.arr[2] = 0;
	printf("%T\n",&a);
	printf("%x\n",&(a.size));
	printf("%x\n",&(a.ch));
	printf("%x\n",a.arr);
	int *t = &a;
	printf("%x\n",t);
	t++;
	printf("%x\n",t);
	printf("%x\n",*t);
	t++;
	printf("%x\n",t);
	printf("main.c:\tmain is start ...\n");
	printf("%T\n",&a);
	mips_init();
	panic("main is over is error!");

	return 0;
}
