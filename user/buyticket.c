//
// Created by 李文豪 on 2022/6/29.
//

#include "lib.h"
void *buy(void *args) {
    sem_t *mutex = (sem_t *)((u_int *)args)[0];
    int *b = (int *)((u_int *)args)[1];
    int thread = (int)((u_int *)args)[2];
    writef("thread%d\n", thread);
    int c;
    int exitflag = 0;
    while (1) {
        sem_wait(mutex);

        if (*b > 0) {
            c = *b;
            *b = *b - 1;
            writef("thread%d buy ticket %d, now have %d tickets\n",thread,c,*b);
        }
        if (*b == 0) {
            exitflag = 1;
        } else if (*b < 0) {
            user_panic("panic at thread%d, tickets are %d\n",thread,*b);
        }

        sem_post(mutex);
        if (exitflag) {
            break;
        }
        int time = 0;
        while (time < 10) {
            time++;
//            writef("thread%d wait for next buy %d ...\n", thread, time);
        }
    }
    pthread_exit(0);
}

void *buy2(void *args) {
    sem_t *mutex = (sem_t *)((u_int *)args)[0];
    int *b = (int *)((u_int *)args)[1];
    int thread = (int)((u_int *)args)[2];
    int c;
    int exitflag = 0;
    while (1) {
        if (sem_trywait(mutex) == 0) {
            if (*b > 0) {
                c = *b;
                *b = *b - 1;
                writef("thread%d buy ticket %d, now have %d tickets\n",thread,c,*b);
            }
            if (*b == 0) {
                exitflag = 1;
            } else if (*b < 0) {
                user_panic("panic at thread%d, tickets are %d\n",thread,*b);
            }
            sem_post(mutex);
        } else {
//            writef("other thread buy, thread%d doesn't wait\n", thread);
        }
        if (exitflag) {
            break;
        }
    }
    pthread_exit(0);
}
void umain() {
    writef("umain thread\n");
    u_int arg1[3];
    u_int arg2[3];
    u_int arg3[3];
    sem_t mutex;
    sem_init(&mutex,0,1);
    arg1[0] = &mutex;
    arg2[0] = &mutex;
    arg3[0] = &mutex;
    int sum = 30;
    arg1[1] = &sum;
    arg2[1] = &sum;
    arg3[1] = &sum;
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    arg1[2] = 1;
    pthread_create(&thread1,NULL,buy,(void *)arg1);
    arg2[2] = 2;
    pthread_create(&thread2,NULL,buy,(void *)arg2);
    arg3[2] = 3;
    pthread_create(&thread3,NULL,buy2,(void *)arg3);
}
