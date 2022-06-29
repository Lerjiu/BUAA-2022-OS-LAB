//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"
#include "../include/env.h"

void *run1(void *arg) {
    writef("thread 1\n");
    int oldvalue;
    pthread_setcancelstate(THREAD_CAN_BE_CANCELED, &oldvalue);
    writef("thread1: old state is %d\n", oldvalue);
    int b = 0;
    while (b < 20) {
        b++;
        writef("thread1: b is %d\n", b);
    }
    while (1);
}

void *run2(void *arg) {
    writef("thread 2\n");
    int oldvalue;
    pthread_setcanceltype(THREAD_CANCEL_POINT, &oldvalue);
    writef("thread2: old type is %d\n", oldvalue);
    pthread_setcancelstate(THREAD_CAN_BE_CANCELED, &oldvalue);
    writef("thread2: old state is %d\n", oldvalue);
//    int c = 0;
//    while (c < 20) {
//        writef("thread2: c is %d\n", c);
//    }
    pthread_testcancel();
    int b = 0;
    while (b < 100) {
        b++;
        writef("thread2: b is %d\n", b);
    }
    pthread_testcancel();
    while (1) {
        writef("thread 2 not cancel!\n");
    }
}

void umain() {
    int a[1];
    a[0] = 1;
    writef("umain thread\n");
    pthread_t thread;
    if (pthread_create(&thread,NULL,run1,(void *)a)) {
        user_panic("create thread fail!\n");
    }
    syscall_yield();
    pthread_cancel(thread);
    if (pthread_create(&thread,NULL,run2,(void *)a)) {
        user_panic("create thread fail 2\n");
    }
    while (pthread_cancel(thread) < 0);
    writef("umain notify thread2 to cancel!\n");
    syscall_yield();
}