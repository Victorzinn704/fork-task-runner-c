#include "ftr/event_stack.h"

#include <stdlib.h>

struct EventStackNode {
  SchedulerEvent event;
  EventStackNode *next;
};

static void event_stack_trim(EventStack *stack) {
  if (stack == NULL || stack->max_size == 0U || stack->size <= stack->max_size) {
    return;
  }

  EventStackNode *previous = NULL;
  EventStackNode *current = stack->head;

  while (current != NULL && current->next != NULL) {
    previous = current;
    current = current->next;
  }

  if (previous != NULL) {
    previous->next = NULL;
  } else {
    stack->head = NULL;
  }

  free(current);
  stack->size--;
}

void event_stack_init(EventStack *stack, size_t max_size) {
  if (stack == NULL) {
    return;
  }

  stack->head = NULL;
  stack->size = 0U;
  stack->max_size = max_size;
}

void event_stack_destroy(EventStack *stack) {
  if (stack == NULL) {
    return;
  }

  EventStackNode *current = stack->head;
  while (current != NULL) {
    EventStackNode *next = current->next;
    free(current);
    current = next;
  }

  event_stack_init(stack, stack->max_size);
}

int event_stack_push(EventStack *stack, const SchedulerEvent *event) {
  if (stack == NULL || event == NULL) {
    return -1;
  }

  EventStackNode *node = malloc(sizeof(*node));
  if (node == NULL) {
    return -1;
  }

  node->event = *event;
  node->next = stack->head;
  stack->head = node;
  stack->size++;
  event_stack_trim(stack);
  return 0;
}

bool event_stack_pop(EventStack *stack, SchedulerEvent *out_event) {
  if (stack == NULL || stack->head == NULL) {
    return false;
  }

  EventStackNode *node = stack->head;
  if (out_event != NULL) {
    *out_event = node->event;
  }

  stack->head = node->next;
  free(node);
  stack->size--;
  return true;
}

bool event_stack_is_empty(const EventStack *stack) {
  return stack == NULL || stack->size == 0U;
}

size_t event_stack_size(const EventStack *stack) {
  return stack == NULL ? 0U : stack->size;
}

void event_stack_for_each(const EventStack *stack, EventStackVisitor visitor, void *context) {
  if (stack == NULL || visitor == NULL) {
    return;
  }

  for (EventStackNode *node = stack->head; node != NULL; node = node->next) {
    visitor(&node->event, context);
  }
}
