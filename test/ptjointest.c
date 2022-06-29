//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"

void *run1(void *args) {
    u_int father_threadid = ((u_int *)args)[0];
    u_int *ret;
    int b = 0;
    writef("thread1 begin\n");
    if (pthread_join(father_threadid,&ret) < 0) {
        writef("thread1 fail join!\n");
        while (b < 10) {
            b++;
            writef("thread1: b is %d\n", b);
        }
        pthread_exit(0);
    }
    writef("thread1: umain is end, ret is %d\n",*ret);
}

void *run2(void *args) {
    u_int father_threadid = ((u_int *)args)[0];
    u_int *ret;
    int b = 0;
    writef("thread2 begin\n");
    if (pthread_join(father_threadid,&ret) < 0) {
        writef("thread2 fail join!\n");
        while (b < 10) {
            b++;
            writef("thread2: b is %d\n", b);
        }
        pthread_exit(0);
    }

    writef("thread2: umain is end, ret is %d\n",*ret);
}

void *run3(void *args) {
    u_int father_threadid = ((u_int *)args)[0];
    u_int *ret;
    int b = 0;
    writef("threaad3 begin\n");
    if (pthread_join(father_threadid,&ret) < 0) {
        writef("thread3 fail join!\n");
        while (b < 10) {
            b++;
            writef("thread3: b is %d\n", b);
        }
        pthread_exit(0);
    }
    writef("thread3: umain is end, ret is %d\n",*ret);
}


void umain() {
    u_int a[1];
    u_int ret = -2;
    a[0] = syscall_getthreadid();
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_detach(a[0]);
    pthread_create(&thread1,NULL,run1,(void *)a);
    pthread_create(&thread2,NULL,run2,(void *)a);
    pthread_create(&thread3,NULL,run3,(void *)a);
    int b = 0;
    while (b < 10) {
        ++b;
        writef("umain thread: b is %d\n",b);
        syscall_yield();
    }

}
