#ifndef FTR_TASK_LOADER_H
#define FTR_TASK_LOADER_H

#include "ftr/task_queue.h"

int task_loader_load_file(const char *path, TaskQueue *queue);

#endif
