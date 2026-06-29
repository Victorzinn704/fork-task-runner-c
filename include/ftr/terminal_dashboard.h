#ifndef FTR_TERMINAL_DASHBOARD_H
#define FTR_TERMINAL_DASHBOARD_H

#include "ftr/event_stack.h"
#include "ftr/process_list.h"
#include "ftr/scheduler.h"
#include "ftr/task_queue.h"

void terminal_dashboard_render(const TaskQueue *pending, const ProcessList *active,
                               const EventStack *history, const SchedulerStats *stats,
                               const SchedulerConfig *config);

void terminal_dashboard_print_summary(const SchedulerStats *stats);

#endif
