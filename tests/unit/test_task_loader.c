#include "ftr/task_loader.h"
#include "tests/test_macros.h"

#include <stdio.h>

#ifndef FTR_FIXTURE_DIR
#define FTR_FIXTURE_DIR "tests/fixtures"
#endif

static void fixture_path(char *buffer, size_t size, const char *file_name) {
  (void)snprintf(buffer, size, "%s/%s", FTR_FIXTURE_DIR, file_name);
}

static int loader_accepts_valid_tasks(void) {
  char path[512];
  fixture_path(path, sizeof(path), "tasks-success.txt");

  TaskQueue queue;
  task_queue_init(&queue);
  TEST_ASSERT(task_loader_load_file(path, &queue) == 0);
  TEST_ASSERT(task_queue_size(&queue) == 3U);
  task_queue_destroy(&queue);
  return 0;
}

static int loader_rejects_comments_only_file(void) {
  char path[512];
  fixture_path(path, sizeof(path), "tasks-comments-only.txt");

  TaskQueue queue;
  task_queue_init(&queue);
  TEST_ASSERT(task_loader_load_file(path, &queue) != 0);
  TEST_ASSERT(task_queue_is_empty(&queue));
  task_queue_destroy(&queue);
  return 0;
}

static int loader_rejects_malformed_line(void) {
  char path[512];
  fixture_path(path, sizeof(path), "tasks-malformed.txt");

  TaskQueue queue;
  task_queue_init(&queue);
  TEST_ASSERT(task_loader_load_file(path, &queue) != 0);
  task_queue_destroy(&queue);
  return 0;
}

static int loader_rejects_invalid_timeout(void) {
  char path[512];
  fixture_path(path, sizeof(path), "tasks-invalid-timeout.txt");

  TaskQueue queue;
  task_queue_init(&queue);
  TEST_ASSERT(task_loader_load_file(path, &queue) != 0);
  task_queue_destroy(&queue);
  return 0;
}

int main(void) {
  RUN_TEST(loader_accepts_valid_tasks);
  RUN_TEST(loader_rejects_comments_only_file);
  RUN_TEST(loader_rejects_malformed_line);
  RUN_TEST(loader_rejects_invalid_timeout);
  return 0;
}
