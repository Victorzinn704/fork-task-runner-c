#include "ftr/scheduler.h"
#include "ftr/task_loader.h"
#include "ftr/terminal_dashboard.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FTR_VERSION
#define FTR_VERSION "0.1.0"
#endif

static void print_usage(FILE *stream, const char *program) {
  fprintf(stream,
          "Uso: %s <arquivo-tarefas> [max-parallel] [--no-dashboard]\n"
          "\n"
          "Opcoes:\n"
          "  -h, --help       Mostra esta ajuda.\n"
          "  -V, --version    Mostra a versao do programa.\n"
          "  --no-dashboard   Desativa o painel ANSI e imprime apenas o relatorio final.\n"
          "\n"
          "Formato do arquivo:\n"
          "  id|nome|timeout_segundos|comando\n"
          "\n"
          "Exemplo:\n"
          "  1|Listar arquivos|5|ls -la\n",
          program);
}

static int is_help_option(const char *value) {
  return value != NULL && (strcmp(value, "-h") == 0 || strcmp(value, "--help") == 0);
}

static int is_version_option(const char *value) {
  return value != NULL && (strcmp(value, "-V") == 0 || strcmp(value, "--version") == 0);
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

static int parse_scheduler_config(int argc, char **argv, SchedulerConfig *config) {
  if (argc < 2 || argv[1][0] == '-' || config == NULL) {
    return -1;
  }

  *config = scheduler_default_config();
  for (int index = 2; index < argc; index++) {
    if (strcmp(argv[index], "--no-dashboard") == 0) {
      config->dashboard_enabled = false;
      continue;
    }

    if (parse_max_parallel(argv[index], &config->max_parallel) != 0) {
      return -1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc == 2 && is_help_option(argv[1])) {
    print_usage(stdout, argv[0]);
    return 0;
  }

  if (argc == 2 && is_version_option(argv[1])) {
    printf("fork-task-runner %s\n", FTR_VERSION);
    return 0;
  }

  SchedulerConfig config;
  if (parse_scheduler_config(argc, argv, &config) != 0) {
    print_usage(stderr, argv[0]);
    return 2;
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
