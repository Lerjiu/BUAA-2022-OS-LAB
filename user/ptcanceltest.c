//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"
#include "../include/env.h"

void *run1(void *arg) {
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
    int oldvalue;
    pthread_setcanceltype(THREAD_CANCEL_POINT, &oldvalue);
    writef("thread2: old type is %d\n", oldvalue);
    pthread_setcancelstate(THREAD_CAN_BE_CANCELED, &oldvalue);
    writef("thread2: old state is %d\n", oldvalue);
    int b = 0;
    pthread_testcancel();
    while (b < 100) {
        b++;
        writef("thread2: b is %d\n", b);
    }
    pthread_testcancel();
}

void umain() {
    int a[1];
    a[0] = 1;

    pthread_t thread;
    if (pthread_create(&thread,NULL,test,(void *)a)) {
        user_panic("create thread fail!\n");
    }
    syscall_yield();
    pthread_cancel(thread);
    if (pthread_create(&thread,NULL,test2,(void *)a)) {
        user_panic("create thread fail 2\n");
    }
    while (pthread_cancel(thread) < 0);
    writef("umain notify thread2 to cancel!\n");
    syscall_yield();
}