//
// Created by 李文豪 on 2022/5/16.
//

#ifndef SCHED_SCHED_H
#define SCHED_SCHED_H
void FCFS (
        int number_of_jobs,
        const int job_submitted_time [],
        const int job_required_time [],
        int job_sched_start []
);

void SJF (
        int number_of_jobs,
        const int job_submitted_time [],
        const int job_required_time [],
        int job_sched_start []
);
#endif //SCHED_SCHED_H

