//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"

void *run(void *arg) {
    int ret = -2;
    writef("run a thread\n");
    writef("new threadid is 0x%x\n",syscall_getthreadid());
    pthread_exit(&ret);
    writef("new thread not exit!\n");
}

void umain() {
    writef("umain thread\n");
    pthread_t new;
    int arg = 0;
    pthread_create(&new,NULL,run,(void *)(&arg));

    while ((int)env->env_threads[new&0x7].tcb_status != ENV_FREE) {
        writef("new thread's tcb is not free\n");
    }
    writef("new thread exit ret is %d\n",*((int *)env->env_threads[new&0x7].tcb_exit_ptr));
}
