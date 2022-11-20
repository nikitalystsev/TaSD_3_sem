#ifndef __QUEUE_ARRAY_H__
#define __QUEUE_ARRAY_H__

#include "defines.h"

typedef struct queue_array_t queue_array_t;
typedef struct arr_elem_t arr_elem_t;

struct arr_elem_t
{
    double arrival_time;
    double processing_time;
    int8_t count_iter;
};

struct queue_array_t
{
    arr_elem_t *queue_array; // массив заявок
    int8_t p_in;        // индекс очередного добавленного элемента
    int8_t p_out;       // индекс очередного исключающего элемента
    int32_t size;        // размер массива заявок
};

int create_queue_array(queue_array_t *queue_array);

void push_back_queue_array(queue_array_t *queue_array,
                           arr_elem_t *data);

void shift(queue_array_t *queue_array);

arr_elem_t pop_front_queue_array(queue_array_t *queue_array);

void print_queue_array(queue_array_t *queue_array);

#endif
