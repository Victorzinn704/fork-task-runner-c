#ifndef FTR_TASK_QUEUE_H
#define FTR_TASK_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#include "ftr/task.h"

typedef struct TaskQueueNode TaskQueueNode;

typedef struct TaskQueue {
  TaskQueueNode *head;
  TaskQueueNode *tail;
  size_t size;
} TaskQueue;

typedef void (*TaskQueueVisitor)(const Task *task, void *context);

void task_queue_init(TaskQueue *queue);
void task_queue_destroy(TaskQueue *queue);
int task_queue_enqueue(TaskQueue *queue, const Task *task);
bool task_queue_dequeue(TaskQueue *queue, Task *out_task);
bool task_queue_is_empty(const TaskQueue *queue);
size_t task_queue_size(const TaskQueue *queue);
void task_queue_for_each(const TaskQueue *queue, TaskQueueVisitor visitor, void *context);

#endif
