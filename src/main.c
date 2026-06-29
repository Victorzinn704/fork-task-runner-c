#include "ftr/scheduler.h"
#include "ftr/task_loader.h"
#include "ftr/terminal_dashboard.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *program) {
  fprintf(stderr,
          "Uso: %s <arquivo-tarefas> [max-parallel] [--no-dashboard]\n"
          "\n"
          "Formato do arquivo:\n"
          "  id|nome|timeout_segundos|comando\n"
          "\n"
          "Exemplo:\n"
          "  1|Listar arquivos|5|ls -la\n",
          program);
}

static int parse_max_parallel(const char *value, size_t *out) {
  if (value == NULL || out == NULL) {
    return -1;
  }

  errno = 0;
  char *end = NULL;
  unsigned long parsed = strtoul(value, &end, 10);
  if (errno != 0 || end == value || *end != '\0' || parsed == 0UL) {
    return -1;
  }

  *out = (size_t)parsed;
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 2;
  }

  SchedulerConfig config = scheduler_default_config();
  for (int index = 2; index < argc; index++) {
    if (strcmp(argv[index], "--no-dashboard") == 0) {
      config.dashboard_enabled = false;
      continue;
    }

    if (parse_max_parallel(argv[index], &config.max_parallel) != 0) {
      print_usage(argv[0]);
      return 2;
    }
  }

  TaskQueue queue;
  task_queue_init(&queue);

  if (task_loader_load_file(argv[1], &queue) != 0) {
    fprintf(stderr, "erro: nao foi possivel carregar tarefas de %s\n", argv[1]);
    task_queue_destroy(&queue);
    return 1;
  }

  SchedulerStats stats;
  int result = scheduler_run(&queue, &config, &stats);
  task_queue_destroy(&queue);

  if (!config.dashboard_enabled) {
    terminal_dashboard_print_summary(&stats);
  }

  return result;
}
