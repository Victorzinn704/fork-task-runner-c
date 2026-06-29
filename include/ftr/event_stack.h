#ifndef FTR_EVENT_STACK_H
#define FTR_EVENT_STACK_H

#include <stdbool.h>
#include <stddef.h>

#include "ftr/task.h"

typedef struct EventStackNode EventStackNode;

typedef struct EventStack {
  EventStackNode *head;
  size_t size;
  size_t max_size;
} EventStack;

typedef void (*EventStackVisitor)(const SchedulerEvent *event, void *context);

void event_stack_init(EventStack *stack, size_t max_size);
void event_stack_destroy(EventStack *stack);
int event_stack_push(EventStack *stack, const SchedulerEvent *event);
bool event_stack_pop(EventStack *stack, SchedulerEvent *out_event);
bool event_stack_is_empty(const EventStack *stack);
size_t event_stack_size(const EventStack *stack);
void event_stack_for_each(const EventStack *stack, EventStackVisitor visitor, void *context);

#endif
