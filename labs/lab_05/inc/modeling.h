#ifndef __MODELING_H__
#define __MODELING_H__

#include "defines.h"
#include "queue_array.h"
#include "queue_list.h"
#include "process.h"

typedef struct param_t param_t;
typedef struct model_t model_t;

struct param_t
{
    double min_add_time;
    double max_add_time;
    double min_process_time;
    double max_process_time;
    int count_appl;
    int count_print_appl;
};

struct model_t
{
    int curr_len_queue;
    double av_len_queue;
    double model_time;
    double input_time;
    double output_time;
    double downtime;
    int count_input;
    int count_output;
    int count_calls_machine;
};

void fill_default_param(param_t *const param);

int model_arr(const param_t *const param);

int model_list(const param_t *const param);

#endif
