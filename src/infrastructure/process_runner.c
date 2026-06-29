#include "ftr/process_runner.h"

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int process_runner_spawn(const Task *task, pid_t *out_pid) {
  if (task == NULL || out_pid == NULL || task->command[0] == '\0') {
    return -1;
  }

  fflush(NULL);

  pid_t pid = fork();
  if (pid < 0) {
    return -1;
  }

  if (pid == 0) {
    (void)setpgid(0, 0);
    execl("/bin/sh", "sh", "-c", task->command, (char *)NULL);
    _exit(127);
  }

  *out_pid = pid;
  return 0;
}

int process_runner_terminate_group(pid_t pid) {
  if (pid <= 0) {
    return -1;
  }

  if (kill(-pid, SIGKILL) == 0) {
    return 0;
  }

  return kill(pid, SIGKILL);
}
