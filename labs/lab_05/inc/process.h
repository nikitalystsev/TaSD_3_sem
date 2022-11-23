#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "defines.h"
#include "queue_array.h"
#include "queue_list.h"
#include "modeling.h"

double random_double(double min, double max);

int read_work_num(int *const number);

int do_modeling(void);

int do_action(void);

#endif
