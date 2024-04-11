#include "system.h"

#define NPROC 256 // currently only 256 tasks are allowed

proc task_struct[NPROC]; // 0 for current running process!