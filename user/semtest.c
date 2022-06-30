//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"

void *run1(void *args) {
    writef("thread1\n");
    sem_t *sem = (sem_t *)((u_int *)args)[0];
    int a;
    int r;
    for (a = 0; a < 2; ++a) {
        r = sem_wait(sem);
        if (r < 0) {
            user_panic("fail P at thread2: %d\n",r);
        }
        writef("thread1 P! now a is %d\n",a + 1);
    }
}

void *run2(void *args) {
    writef("thread2\n");
    sem_t *sem = (sem_t *)((u_int *)args)[0];
    int b;
    int r;
    for (b = 0; b < 3; ++b) {
        r = sem_wait(sem);
        if (r < 0) {
            user_panic("fail P at thread2: %d\n",r);
        }
        writef("thread2 P! now b is %d\n",b + 1);
    }
}

void *run3(void *args) {
    writef("thread3\n");
    sem_t *sem = (sem_t *)((u_int *)args)[0];
    int c;
    int r;
    for(c = 0; c < 5; ++c) {
        r = sem_wait(sem);
        if (r < 0) {
            user_panic("fail P at thread3: %d\n",r);
        }
        writef("thread3 P! now c is %d\n",c + 1);
    }
}

void umain() {
    writef("umain thread\n");
    u_int a[1];
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    sem_t mysem;
    sem_init(&mysem,0,1);
    a[0] = &mysem;
    int r;
    pthread_create(&thread1,NULL,run1,(void *)a);
    pthread_create(&thread2,NULL,run2,(void *)a);
    pthread_create(&thread3,NULL,run3,(void *)a);
    int value;
    r = sem_getvalue(&mysem,&value);
    if (r < 0) {
        user_panic("get value error: %d\n",r);
    }
    int i = 0;
    for (i = 0; i < 9; ++i) {
        if (value == 0) {
            writef("umain thread post the %d!\n", i+1);
            sem_post(&mysem);
        }
        syscall_yield();
        r = sem_getvalue(&mysem,&value);
        if (r < 0) {
            user_panic("r is %d\n",r);
        }
        writef("after post the %d, sem value is %d\n", i+1, value);
    }
}
