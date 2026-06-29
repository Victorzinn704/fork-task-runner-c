#include "ftr/terminal_dashboard.h"

#include <stdio.h>
#include <time.h>

#define DASHBOARD_ITEM_LIMIT 8U

typedef struct RenderLimit {
  size_t printed;
  size_t limit;
} RenderLimit;

static void print_pending_task(const Task *task, void *context) {
  RenderLimit *limit = context;
  if (limit->printed >= limit->limit) {
    return;
  }

  printf("[%02d] %s | timeout %us | %s\n", task->id, task->name, task->timeout_seconds,
         task->command);
  limit->printed++;
}

static void print_active_process(const ProcessRecord *record, void *context) {
  RenderLimit *limit = context;
  if (limit->printed >= limit->limit) {
    return;
  }

  time_t now = time(NULL);
  long elapsed = (long)difftime(now, record->started_at);
  printf("PID %-8ld | TASK %02d | RUNNING | %lds/%us | %s\n", (long)record->pid, record->task_id,
         elapsed, record->timeout_seconds, record->name);
  limit->printed++;
}

static void print_history_event(const SchedulerEvent *event, void *context) {
  RenderLimit *limit = context;
  if (limit->printed >= limit->limit) {
    return;
  }

  printf("[%s] task %02d | pid %-8ld | exit %d | %s\n", scheduler_event_type_to_string(event->type),
         event->task_id, (long)event->pid, event->exit_code, event->message);
  limit->printed++;
}

void terminal_dashboard_render(const TaskQueue *pending, const ProcessList *active,
                               const EventStack *history, const SchedulerStats *stats,
                               const SchedulerConfig *config) {
  if (pending == NULL || active == NULL || history == NULL || stats == NULL || config == NULL) {
    return;
  }

  printf("\033[2J\033[H");
  printf("================== FORK TASK RUNNER ==================\n\n");
  printf("MAX PARALLEL: %zu | REFRESH: %ums\n\n", config->max_parallel, config->refresh_millis);

  printf("PENDING QUEUE (%zu)\n", task_queue_size(pending));
  RenderLimit pending_limit = {0U, DASHBOARD_ITEM_LIMIT};
  task_queue_for_each(pending, print_pending_task, &pending_limit);
  if (pending_limit.printed == 0U) {
    printf("sem tarefas pendentes\n");
  }

  printf("\nACTIVE PROCESSES (%zu)\n", process_list_size(active));
  RenderLimit active_limit = {0U, DASHBOARD_ITEM_LIMIT};
  process_list_for_each(active, print_active_process, &active_limit);
  if (active_limit.printed == 0U) {
    printf("sem processos ativos\n");
  }

  printf("\nRECENT EVENTS (%zu)\n", event_stack_size(history));
  RenderLimit history_limit = {0U, DASHBOARD_ITEM_LIMIT};
  event_stack_for_each(history, print_history_event, &history_limit);
  if (history_limit.printed == 0U) {
    printf("sem eventos registrados\n");
  }

  printf("\nSUMMARY\n");
  printf("total: %zu | started: %zu | finished: %zu | failed: %zu | timeout: %zu\n", stats->total,
         stats->started, stats->finished, stats->failed, stats->timed_out);
  printf("======================================================\n");
  fflush(stdout);
}

void terminal_dashboard_print_summary(const SchedulerStats *stats) {
  if (stats == NULL) {
    return;
  }

  long elapsed = 0L;
  if (stats->started_at != 0 && stats->finished_at != 0) {
    elapsed = (long)difftime(stats->finished_at, stats->started_at);
  }

  printf("\n================== FINAL REPORT ==================\n");
  printf("Total tasks: %zu\n", stats->total);
  printf("Started:     %zu\n", stats->started);
  printf("Finished:    %zu\n", stats->finished);
  printf("Failed:      %zu\n", stats->failed);
  printf("Timeout:     %zu\n", stats->timed_out);
  printf("Elapsed:     %lds\n", elapsed);
  printf("==================================================\n");
}
