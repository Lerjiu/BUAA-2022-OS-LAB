//
// Created by 李文豪 on 2022/6/28.
//

#include "lib.h"
#include "../include/types.h"
int shared = 0;
void *run(void *arg) {
    int count = 0;
    writef("run a thread\n");
    while (1) {
        if (count == 100) {
            writef("count is 100 and new thread break");
            break;
        }
        count++;
        shared++;
        *((int*)arg) += 1;
        writef("new thread:0x%x count:%d arg:%d shared:%d\n",syscall_getthreadid(), count, *((int*)arg), shared);
    }
}

void umain() {
    int r;
    writef("umain thread\n");

    int count = 0;
    int arg = 0;
    pthread_t new;

    r = pthread_create(&new,NULL, run,(void *)(&arg));
    if (!r) {
        while (1) {
            if (count == 100) {
                writef("count is 100 and umain thread break");
                break;
            }
            count++;
            shared++;
            arg++;
            writef("umain thread:0x%x count:%d arg:%d shared:%d\n", syscall_getthreadid(), count, arg, shared);
        }
    }
}