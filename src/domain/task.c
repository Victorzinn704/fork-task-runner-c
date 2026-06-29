#include "ftr/task.h"

#include <stdio.h>
#include <string.h>

int task_init(Task *task, int id, const char *name, const char *command,
              unsigned int timeout_seconds) {
  if (task == NULL || name == NULL || command == NULL || id <= 0 || timeout_seconds == 0U) {
    return -1;
  }

  task->id = id;
  task->timeout_seconds = timeout_seconds;
  task->status = TASK_STATUS_QUEUED;
  task->exit_code = 0;

  snprintf(task->name, sizeof(task->name), "%s", name);
  snprintf(task->command, sizeof(task->command), "%s", command);

  if (task->name[0] == '\0' || task->command[0] == '\0') {
    return -1;
  }

  return 0;
}

const char *task_status_to_string(TaskStatus status) {
  switch (status) {
  case TASK_STATUS_QUEUED:
    return "QUEUED";
  case TASK_STATUS_RUNNING:
    return "RUNNING";
  case TASK_STATUS_FINISHED:
    return "FINISHED";
  case TASK_STATUS_FAILED:
    return "FAILED";
  case TASK_STATUS_TIMEOUT:
    return "TIMEOUT";
  default:
    return "UNKNOWN";
  }
}

const char *scheduler_event_type_to_string(SchedulerEventType type) {
  switch (type) {
  case SCHEDULER_EVENT_QUEUED:
    return "QUEUED";
  case SCHEDULER_EVENT_STARTED:
    return "STARTED";
  case SCHEDULER_EVENT_FINISHED:
    return "FINISHED";
  case SCHEDULER_EVENT_FAILED:
    return "FAILED";
  case SCHEDULER_EVENT_TIMEOUT:
    return "TIMEOUT";
  default:
    return "UNKNOWN";
  }
}
