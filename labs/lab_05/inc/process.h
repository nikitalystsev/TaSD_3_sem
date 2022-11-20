#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "defines.h"
#include "queue_array.h"
#include "queue_list.h"
#include "gen_number.h"

typedef struct parametres_t parametres_t;
typedef struct data_modeling_t data_modeling_t;

struct parametres_t
{
    double min_time_add;
    double max_time_add;
    double min_time_processing;
    double max_time_processing;
    int16_t count_application;
    int8_t count_print_application;
};

struct data_modeling_t
{
    int16_t current_length_queue;
    double average_length_queue;
    double total_simulation_time;
    double time_input_all_application;
    double time_output_all_application;
    double downtime_machine;
    int16_t total_input_application;
    int16_t total_output_application;
    int16_t count_calls_machine;
};

void fill_default_parametres(parametres_t *const parametres);

int modeling_process_arr(const parametres_t *const parametres);

int modeling_process_list(const parametres_t *const parametres);

#endif
