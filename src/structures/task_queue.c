#include "ftr/task_queue.h"

#include <stdlib.h>

struct TaskQueueNode {
  Task task;
  TaskQueueNode *next;
};

void task_queue_init(TaskQueue *queue) {
  if (queue == NULL) {
    return;
  }

  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0U;
}

void task_queue_destroy(TaskQueue *queue) {
  if (queue == NULL) {
    return;
  }

  TaskQueueNode *current = queue->head;
  while (current != NULL) {
    TaskQueueNode *next = current->next;
    free(current);
    current = next;
  }

  task_queue_init(queue);
}

int task_queue_enqueue(TaskQueue *queue, const Task *task) {
  if (queue == NULL || task == NULL) {
    return -1;
  }

  TaskQueueNode *node = malloc(sizeof(*node));
  if (node == NULL) {
    return -1;
  }

  node->task = *task;
  node->next = NULL;

  if (queue->tail != NULL) {
    queue->tail->next = node;
  } else {
    queue->head = node;
  }

  queue->tail = node;
  queue->size++;
  return 0;
}

bool task_queue_dequeue(TaskQueue *queue, Task *out_task) {
  if (queue == NULL || queue->head == NULL) {
    return false;
  }

  TaskQueueNode *node = queue->head;
  if (out_task != NULL) {
    *out_task = node->task;
  }

  queue->head = node->next;
  if (queue->head == NULL) {
    queue->tail = NULL;
  }

  free(node);
  queue->size--;
  return true;
}

bool task_queue_is_empty(const TaskQueue *queue) {
  return queue == NULL || queue->size == 0U;
}

size_t task_queue_size(const TaskQueue *queue) {
  return queue == NULL ? 0U : queue->size;
}

void task_queue_for_each(const TaskQueue *queue, TaskQueueVisitor visitor, void *context) {
  if (queue == NULL || visitor == NULL) {
    return;
  }

  for (TaskQueueNode *node = queue->head; node != NULL; node = node->next) {
    visitor(&node->task, context);
  }
}
