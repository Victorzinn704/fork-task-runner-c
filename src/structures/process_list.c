#include "ftr/process_list.h"

#include <stdlib.h>

struct ProcessListNode {
  ProcessRecord record;
  ProcessListNode *next;
};

void process_list_init(ProcessList *list) {
  if (list == NULL) {
    return;
  }

  list->head = NULL;
  list->size = 0U;
}

void process_list_destroy(ProcessList *list) {
  if (list == NULL) {
    return;
  }

  ProcessListNode *current = list->head;
  while (current != NULL) {
    ProcessListNode *next = current->next;
    free(current);
    current = next;
  }

  process_list_init(list);
}

int process_list_add(ProcessList *list, const ProcessRecord *record) {
  if (list == NULL || record == NULL || record->pid <= 0) {
    return -1;
  }

  ProcessListNode *node = malloc(sizeof(*node));
  if (node == NULL) {
    return -1;
  }

  node->record = *record;
  node->next = list->head;
  list->head = node;
  list->size++;
  return 0;
}

bool process_list_remove_by_pid(ProcessList *list, pid_t pid, ProcessRecord *out_record) {
  if (list == NULL || pid <= 0) {
    return false;
  }

  ProcessListNode *previous = NULL;
  ProcessListNode *current = list->head;

  while (current != NULL) {
    if (current->record.pid == pid) {
      if (out_record != NULL) {
        *out_record = current->record;
      }

      if (previous == NULL) {
        list->head = current->next;
      } else {
        previous->next = current->next;
      }

      free(current);
      list->size--;
      return true;
    }

    previous = current;
    current = current->next;
  }

  return false;
}

bool process_list_find_by_pid(const ProcessList *list, pid_t pid, ProcessRecord *out_record) {
  if (list == NULL || pid <= 0) {
    return false;
  }

  for (ProcessListNode *node = list->head; node != NULL; node = node->next) {
    if (node->record.pid == pid) {
      if (out_record != NULL) {
        *out_record = node->record;
      }

      return true;
    }
  }

  return false;
}

bool process_list_find_timed_out(const ProcessList *list, time_t now, ProcessRecord *out_record) {
  if (list == NULL) {
    return false;
  }

  for (ProcessListNode *node = list->head; node != NULL; node = node->next) {
    double elapsed = difftime(now, node->record.started_at);
    if (node->record.timeout_seconds > 0U && elapsed >= (double)node->record.timeout_seconds) {
      if (out_record != NULL) {
        *out_record = node->record;
      }

      return true;
    }
  }

  return false;
}

size_t process_list_size(const ProcessList *list) {
  return list == NULL ? 0U : list->size;
}

void process_list_for_each(const ProcessList *list, ProcessListVisitor visitor, void *context) {
  if (list == NULL || visitor == NULL) {
    return;
  }

  for (ProcessListNode *node = list->head; node != NULL; node = node->next) {
    visitor(&node->record, context);
  }
}
