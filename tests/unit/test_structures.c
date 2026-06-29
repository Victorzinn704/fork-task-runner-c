#include "ftr/event_stack.h"
#include "ftr/process_list.h"
#include "ftr/task_queue.h"
#include "tests/test_macros.h"

#include <sys/types.h>
#include <time.h>

static Task make_task(int id, const char *name) {
  Task task;
  (void)task_init(&task, id, name, "true", 5U);
  return task;
}

static int queue_preserves_fifo_order(void) {
  TaskQueue queue;
  task_queue_init(&queue);

  Task first = make_task(1, "primeira");
  Task second = make_task(2, "segunda");
  TEST_ASSERT(task_queue_enqueue(&queue, &first) == 0);
  TEST_ASSERT(task_queue_enqueue(&queue, &second) == 0);
  TEST_ASSERT(task_queue_size(&queue) == 2U);

  Task out;
  TEST_ASSERT(task_queue_dequeue(&queue, &out));
  TEST_ASSERT(out.id == 1);
  TEST_ASSERT(task_queue_dequeue(&queue, &out));
  TEST_ASSERT(out.id == 2);
  TEST_ASSERT(task_queue_is_empty(&queue));

  task_queue_destroy(&queue);
  return 0;
}

static int event_stack_returns_recent_event_first(void) {
  EventStack stack;
  event_stack_init(&stack, 2U);

  SchedulerEvent first = {.type = SCHEDULER_EVENT_STARTED, .task_id = 1, .pid = 10};
  SchedulerEvent second = {.type = SCHEDULER_EVENT_FINISHED, .task_id = 2, .pid = 11};
  SchedulerEvent third = {.type = SCHEDULER_EVENT_FAILED, .task_id = 3, .pid = 12};

  TEST_ASSERT(event_stack_push(&stack, &first) == 0);
  TEST_ASSERT(event_stack_push(&stack, &second) == 0);
  TEST_ASSERT(event_stack_push(&stack, &third) == 0);
  TEST_ASSERT(event_stack_size(&stack) == 2U);

  SchedulerEvent out;
  TEST_ASSERT(event_stack_pop(&stack, &out));
  TEST_ASSERT(out.task_id == 3);
  TEST_ASSERT(event_stack_pop(&stack, &out));
  TEST_ASSERT(out.task_id == 2);
  TEST_ASSERT(event_stack_is_empty(&stack));

  event_stack_destroy(&stack);
  return 0;
}

static int process_list_removes_by_pid(void) {
  ProcessList list;
  process_list_init(&list);

  ProcessRecord first = {
      .task_id = 1, .pid = 1001, .timeout_seconds = 5U, .started_at = time(NULL)};
  ProcessRecord second = {
      .task_id = 2, .pid = 1002, .timeout_seconds = 5U, .started_at = time(NULL)};

  TEST_ASSERT(process_list_add(&list, &first) == 0);
  TEST_ASSERT(process_list_add(&list, &second) == 0);
  TEST_ASSERT(process_list_size(&list) == 2U);

  ProcessRecord removed;
  TEST_ASSERT(process_list_remove_by_pid(&list, (pid_t)1001, &removed));
  TEST_ASSERT(removed.task_id == 1);
  TEST_ASSERT(process_list_size(&list) == 1U);
  TEST_ASSERT(!process_list_find_by_pid(&list, (pid_t)1001, NULL));
  TEST_ASSERT(process_list_find_by_pid(&list, (pid_t)1002, NULL));

  process_list_destroy(&list);
  return 0;
}

int main(void) {
  RUN_TEST(queue_preserves_fifo_order);
  RUN_TEST(event_stack_returns_recent_event_first);
  RUN_TEST(process_list_removes_by_pid);
  return 0;
}
