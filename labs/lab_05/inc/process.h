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

int queue_arr_size_in_bytes(const queue_array_t *const queue);

int queue_list_size_in_bytes(const queue_list_t *const queue);

int max_int(const int a, const int b);

double max_double(const double a, const double b);


#endif
