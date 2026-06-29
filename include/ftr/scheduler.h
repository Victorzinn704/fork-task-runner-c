#ifndef FTR_SCHEDULER_H
#define FTR_SCHEDULER_H

#include <stdbool.h>
#include <stddef.h>
#include <time.h>

#include "ftr/task_queue.h"

typedef struct SchedulerConfig {
  size_t max_parallel;
  bool dashboard_enabled;
  unsigned int refresh_millis;
  size_t history_limit;
} SchedulerConfig;

typedef struct SchedulerStats {
  size_t total;
  size_t started;
  size_t finished;
  size_t failed;
  size_t timed_out;
  time_t started_at;
  time_t finished_at;
} SchedulerStats;

SchedulerConfig scheduler_default_config(void);
int scheduler_run(TaskQueue *pending, const SchedulerConfig *config, SchedulerStats *stats);

#endif
