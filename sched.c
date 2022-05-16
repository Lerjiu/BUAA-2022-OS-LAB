//
// Created by 李文豪 on 2022/5/16.
//

//#include "stdlib.h"
#include "sched.h"
#define max(x,y) (x)>(y)?(x):(y)

typedef struct PCB {
    int pid;
    int submitted_time;
    int required_time;
}PCB;

int cmp(PCB p1, PCB p2)
{
    if (p1.required_time != p2.required_time) {
        return p1.required_time - p2.required_time;
    } else if (p1.submitted_time != p2.submitted_time) {
        return p1.submitted_time - p2.submitted_time;
    } else {
        return p1.pid - p2.pid;
    }
}

void swap(PCB *p1, PCB *p2)
{
    PCB tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void FCFS(int number_of_jobs,
          const int job_submitted_time [],
          const int job_required_time [],
          int job_sched_start [])
{
    int time = 0;
    int i;
    for (i = 0; i < number_of_jobs; i++) {
        time = max(time, job_submitted_time[i]);
        job_sched_start[i] = time;
        time += job_required_time[i];
    }
}

void SJF(int number_of_jobs,
         const int job_submitted_time [],
         const int job_required_time [],
         int job_sched_start [])
{
    int time = 0;
    int i = 0;
    PCB *progress = (PCB*)malloc(sizeof(PCB) * number_of_jobs);
    for (i = 0; i < number_of_jobs; i++) {
        progress[i].pid = i;
        progress[i].submitted_time = job_submitted_time[i];
        progress[i].required_time = job_required_time[i];
    }

    i = 0;

    while (i < number_of_jobs) {
        if (time < progress[i].submitted_time) {
            time = progress[i].submitted_time;
        }

        int wait_sched = i;
        int index;
        PCB sched_progress = {0x7fffffff, 0x7fffffff, 0x7fffffff};
        while (progress[wait_sched].submitted_time <= time) {
            if (cmp(sched_progress, progress[wait_sched]) > 0) {
                sched_progress = progress[wait_sched];
                index = wait_sched;
            }
            wait_sched++;
        }
        swap(&progress[i], &progress[index]);

        job_sched_start[progress[i].pid] = time;
        time += progress[i].required_time;
        i++;
    }
}
