#ifndef FTR_PROCESS_RUNNER_H
#define FTR_PROCESS_RUNNER_H

#include <sys/types.h>

#include "ftr/task.h"

int process_runner_spawn(const Task *task, pid_t *out_pid);
int process_runner_terminate_group(pid_t pid);

#endif
