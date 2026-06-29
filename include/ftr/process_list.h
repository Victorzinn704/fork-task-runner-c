#ifndef FTR_PROCESS_LIST_H
#define FTR_PROCESS_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <time.h>

#include "ftr/task.h"

typedef struct ProcessRecord {
  int task_id;
  pid_t pid;
  char name[FTR_TASK_NAME_MAX];
  char command[FTR_TASK_COMMAND_MAX];
  unsigned int timeout_seconds;
  time_t started_at;
} ProcessRecord;

typedef struct ProcessListNode ProcessListNode;

typedef struct ProcessList {
  ProcessListNode *head;
  size_t size;
} ProcessList;

typedef void (*ProcessListVisitor)(const ProcessRecord *record, void *context);

void process_list_init(ProcessList *list);
void process_list_destroy(ProcessList *list);
int process_list_add(ProcessList *list, const ProcessRecord *record);
bool process_list_remove_by_pid(ProcessList *list, pid_t pid, ProcessRecord *out_record);
bool process_list_find_by_pid(const ProcessList *list, pid_t pid, ProcessRecord *out_record);
bool process_list_find_timed_out(const ProcessList *list, time_t now, ProcessRecord *out_record);
size_t process_list_size(const ProcessList *list);
void process_list_for_each(const ProcessList *list, ProcessListVisitor visitor, void *context);

#endif
