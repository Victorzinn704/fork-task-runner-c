#ifndef FTR_TASK_H
#define FTR_TASK_H

#include <stddef.h>
#include <sys/types.h>
#include <time.h>

#define FTR_TASK_NAME_MAX 96
#define FTR_TASK_COMMAND_MAX 512
#define FTR_EVENT_MESSAGE_MAX 160

typedef enum TaskStatus {
  TASK_STATUS_QUEUED = 0,
  TASK_STATUS_RUNNING,
  TASK_STATUS_FINISHED,
  TASK_STATUS_FAILED,
  TASK_STATUS_TIMEOUT
} TaskStatus;

typedef enum SchedulerEventType {
  SCHEDULER_EVENT_QUEUED = 0,
  SCHEDULER_EVENT_STARTED,
  SCHEDULER_EVENT_FINISHED,
  SCHEDULER_EVENT_FAILED,
  SCHEDULER_EVENT_TIMEOUT
} SchedulerEventType;

typedef struct Task {
  int id;
  char name[FTR_TASK_NAME_MAX];
  char command[FTR_TASK_COMMAND_MAX];
  unsigned int timeout_seconds;
  TaskStatus status;
  int exit_code;
} Task;

typedef struct SchedulerEvent {
  SchedulerEventType type;
  int task_id;
  pid_t pid;
  int exit_code;
  time_t occurred_at;
  char message[FTR_EVENT_MESSAGE_MAX];
} SchedulerEvent;

int task_init(Task *task, int id, const char *name, const char *command,
              unsigned int timeout_seconds);

const char *task_status_to_string(TaskStatus status);
const char *scheduler_event_type_to_string(SchedulerEventType type);

#endif
