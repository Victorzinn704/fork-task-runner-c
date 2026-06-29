#include "ftr/task_loader.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TASK_LOADER_LINE_MAX 1024

static char *trim(char *value) {
  if (value == NULL) {
    return NULL;
  }

  while (isspace((unsigned char)*value) != 0) {
    value++;
  }

  if (*value == '\0') {
    return value;
  }

  char *end = value + strlen(value) - 1U;
  while (end > value && isspace((unsigned char)*end) != 0) {
    *end = '\0';
    end--;
  }

  return value;
}

static int parse_positive_int(const char *value, int *out) {
  if (value == NULL || out == NULL) {
    return -1;
  }

  char *end = NULL;
  long parsed = strtol(value, &end, 10);
  if (end == value || *trim(end) != '\0' || parsed <= 0L || parsed > 2147483647L) {
    return -1;
  }

  *out = (int)parsed;
  return 0;
}

static int parse_task_line(char *line, Task *task) {
  char *id_part = trim(line);
  if (id_part == NULL || id_part[0] == '\0' || id_part[0] == '#') {
    return 1;
  }

  char *name_part = strchr(id_part, '|');
  if (name_part == NULL) {
    return -1;
  }

  *name_part = '\0';
  name_part++;

  char *timeout_part = strchr(name_part, '|');
  if (timeout_part == NULL) {
    return -1;
  }

  *timeout_part = '\0';
  timeout_part++;

  char *command_part = strchr(timeout_part, '|');
  if (command_part == NULL) {
    return -1;
  }

  *command_part = '\0';
  command_part++;

  int id = 0;
  int timeout = 0;
  if (parse_positive_int(trim(id_part), &id) != 0 ||
      parse_positive_int(trim(timeout_part), &timeout) != 0) {
    return -1;
  }

  return task_init(task, id, trim(name_part), trim(command_part), (unsigned int)timeout);
}

int task_loader_load_file(const char *path, TaskQueue *queue) {
  if (path == NULL || queue == NULL) {
    return -1;
  }

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    return -1;
  }

  char line[TASK_LOADER_LINE_MAX];
  int loaded = 0;
  int result = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    Task task;
    int parse_result = parse_task_line(line, &task);
    if (parse_result == 1) {
      continue;
    }

    if (parse_result != 0 || task_queue_enqueue(queue, &task) != 0) {
      result = -1;
      break;
    }

    loaded++;
  }

  if (fclose(file) != 0) {
    result = -1;
  }

  if (loaded == 0 && result == 0) {
    result = -1;
  }

  return result;
}
