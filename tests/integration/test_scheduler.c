#include "ftr/scheduler.h"
#include "tests/test_macros.h"

static int scheduler_runs_successful_tasks(void) {
  TaskQueue queue;
  task_queue_init(&queue);

  Task first;
  Task second;
  TEST_ASSERT(task_init(&first, 1, "true one", "true", 3U) == 0);
  TEST_ASSERT(task_init(&second, 2, "true two", "true", 3U) == 0);
  TEST_ASSERT(task_queue_enqueue(&queue, &first) == 0);
  TEST_ASSERT(task_queue_enqueue(&queue, &second) == 0);

  SchedulerConfig config = scheduler_default_config();
  config.dashboard_enabled = false;
  config.refresh_millis = 50U;
  config.max_parallel = 2U;

  SchedulerStats stats;
  int result = scheduler_run(&queue, &config, &stats);
  TEST_ASSERT(result == 0);
  TEST_ASSERT(stats.total == 2U);
  TEST_ASSERT(stats.started == 2U);
  TEST_ASSERT(stats.finished == 2U);
  TEST_ASSERT(stats.failed == 0U);
  TEST_ASSERT(stats.timed_out == 0U);

  task_queue_destroy(&queue);
  return 0;
}

static int scheduler_reports_failed_tasks(void) {
  TaskQueue queue;
  task_queue_init(&queue);

  Task task;
  TEST_ASSERT(task_init(&task, 1, "false command", "false", 3U) == 0);
  TEST_ASSERT(task_queue_enqueue(&queue, &task) == 0);

  SchedulerConfig config = scheduler_default_config();
  config.dashboard_enabled = false;
  config.refresh_millis = 50U;
  config.max_parallel = 1U;

  SchedulerStats stats;
  int result = scheduler_run(&queue, &config, &stats);
  TEST_ASSERT(result == 1);
  TEST_ASSERT(stats.started == 1U);
  TEST_ASSERT(stats.finished == 0U);
  TEST_ASSERT(stats.failed == 1U);
  TEST_ASSERT(stats.timed_out == 0U);

  task_queue_destroy(&queue);
  return 0;
}

static int scheduler_reports_timeout_tasks(void) {
  TaskQueue queue;
  task_queue_init(&queue);

  Task task;
  TEST_ASSERT(task_init(&task, 1, "slow command", "sleep 2", 1U) == 0);
  TEST_ASSERT(task_queue_enqueue(&queue, &task) == 0);

  SchedulerConfig config = scheduler_default_config();
  config.dashboard_enabled = false;
  config.refresh_millis = 50U;
  config.max_parallel = 1U;

  SchedulerStats stats;
  int result = scheduler_run(&queue, &config, &stats);
  TEST_ASSERT(result == 1);
  TEST_ASSERT(stats.started == 1U);
  TEST_ASSERT(stats.finished == 0U);
  TEST_ASSERT(stats.failed == 0U);
  TEST_ASSERT(stats.timed_out == 1U);

  task_queue_destroy(&queue);
  return 0;
}

int main(void) {
  RUN_TEST(scheduler_runs_successful_tasks);
  RUN_TEST(scheduler_reports_failed_tasks);
  RUN_TEST(scheduler_reports_timeout_tasks);
  return 0;
}
