#include "ftr/scheduler.h"

#include "ftr/event_stack.h"
#include "ftr/process_list.h"
#include "ftr/process_runner.h"
#include "ftr/terminal_dashboard.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

static void sleep_millis(unsigned int millis) {
  struct timespec requested;
  requested.tv_sec = (time_t)(millis / 1000U);
  requested.tv_nsec = (long)(millis % 1000U) * 1000000L;

  while (nanosleep(&requested, &requested) == -1 && errno == EINTR) {
  }
}

static void push_event(EventStack *history, SchedulerEventType type, const ProcessRecord *record,
                       int exit_code, const char *message) {
  SchedulerEvent event;
  event.type = type;
  event.task_id = record != NULL ? record->task_id : 0;
  event.pid = record != NULL ? record->pid : 0;
  event.exit_code = exit_code;
  event.occurred_at = time(NULL);
  snprintf(event.message, sizeof(event.message), "%s", message != NULL ? message : "");
  (void)event_stack_push(history, &event);
}

static int exit_code_from_status(int status) {
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  }

  if (WIFSIGNALED(status)) {
    return 128 + WTERMSIG(status);
  }

  return -1;
}

static void reap_finished(ProcessList *active, EventStack *history, SchedulerStats *stats) {
  int status = 0;
  pid_t pid = 0;

  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    ProcessRecord record;
    if (!process_list_remove_by_pid(active, pid, &record)) {
      continue;
    }

    int exit_code = exit_code_from_status(status);
    if (WIFEXITED(status) && exit_code == 0) {
      stats->finished++;
      push_event(history, SCHEDULER_EVENT_FINISHED, &record, exit_code,
                 "processo finalizado com sucesso");
    } else {
      stats->failed++;
      push_event(history, SCHEDULER_EVENT_FAILED, &record, exit_code,
                 "processo finalizado com falha");
    }
  }
}

static void wait_after_timeout(pid_t pid) {
  int status = 0;
  while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {
  }
}

static void handle_timeouts(ProcessList *active, EventStack *history, SchedulerStats *stats) {
  ProcessRecord record;
  time_t now = time(NULL);

  while (process_list_find_timed_out(active, now, &record)) {
    (void)process_runner_terminate_group(record.pid);
    wait_after_timeout(record.pid);

    ProcessRecord removed;
    if (process_list_remove_by_pid(active, record.pid, &removed)) {
      stats->timed_out++;
      push_event(history, SCHEDULER_EVENT_TIMEOUT, &removed, 137, "processo encerrado por timeout");
    }
  }
}

static int launch_next_task(TaskQueue *pending, ProcessList *active, EventStack *history,
                            SchedulerStats *stats) {
  Task task;
  if (!task_queue_dequeue(pending, &task)) {
    return 0;
  }

  pid_t pid = 0;
  if (process_runner_spawn(&task, &pid) != 0) {
    ProcessRecord failed = {
        .task_id = task.id,
        .pid = 0,
        .timeout_seconds = task.timeout_seconds,
        .started_at = time(NULL),
    };
    snprintf(failed.name, sizeof(failed.name), "%s", task.name);
    snprintf(failed.command, sizeof(failed.command), "%s", task.command);
    stats->failed++;
    push_event(history, SCHEDULER_EVENT_FAILED, &failed, -1, "fork falhou ao iniciar tarefa");
    return -1;
  }

  ProcessRecord record = {
      .task_id = task.id,
      .pid = pid,
      .timeout_seconds = task.timeout_seconds,
      .started_at = time(NULL),
  };
  snprintf(record.name, sizeof(record.name), "%s", task.name);
  snprintf(record.command, sizeof(record.command), "%s", task.command);

  if (process_list_add(active, &record) != 0) {
    (void)process_runner_terminate_group(pid);
    wait_after_timeout(pid);
    stats->failed++;
    push_event(history, SCHEDULER_EVENT_FAILED, &record, -1, "falha ao registrar processo ativo");
    return -1;
  }

  stats->started++;
  push_event(history, SCHEDULER_EVENT_STARTED, &record, 0, "processo iniciado");
  return 0;
}

SchedulerConfig scheduler_default_config(void) {
  SchedulerConfig config = {
      .max_parallel = 2U,
      .dashboard_enabled = true,
      .refresh_millis = 250U,
      .history_limit = 32U,
  };
  return config;
}

int scheduler_run(TaskQueue *pending, const SchedulerConfig *config, SchedulerStats *stats) {
  if (pending == NULL || config == NULL || stats == NULL || config->max_parallel == 0U) {
    return -1;
  }

  memset(stats, 0, sizeof(*stats));
  stats->total = task_queue_size(pending);
  stats->started_at = time(NULL);

  ProcessList active;
  EventStack history;
  process_list_init(&active);
  event_stack_init(&history, config->history_limit);

  while (!task_queue_is_empty(pending) || process_list_size(&active) > 0U) {
    reap_finished(&active, &history, stats);
    handle_timeouts(&active, &history, stats);

    while (process_list_size(&active) < config->max_parallel && !task_queue_is_empty(pending)) {
      (void)launch_next_task(pending, &active, &history, stats);
    }

    if (config->dashboard_enabled) {
      terminal_dashboard_render(pending, &active, &history, stats, config);
    }

    if (!task_queue_is_empty(pending) || process_list_size(&active) > 0U) {
      sleep_millis(config->refresh_millis);
    }
  }

  stats->finished_at = time(NULL);

  if (config->dashboard_enabled) {
    terminal_dashboard_render(pending, &active, &history, stats, config);
    terminal_dashboard_print_summary(stats);
  }

  event_stack_destroy(&history);
  process_list_destroy(&active);

  return stats->failed == 0U && stats->timed_out == 0U ? 0 : 1;
}
