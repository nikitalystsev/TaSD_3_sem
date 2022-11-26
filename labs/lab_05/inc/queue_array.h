#ifndef __QUEUE_ARRAY_H__
#define __QUEUE_ARRAY_H__

#include "defines.h"

typedef struct queue_array_t queue_array_t;

/// @brief очередь как массив
struct queue_array_t
{
    elem_t *queue_array; // массив заявок
    int p_in;            // индекс очередного добавленного элемента
    int p_out;           // индекс очередного исключающего элемента
    int size;            // размер массива заявок
};

int create_queue_array(queue_array_t *const queue);

int push_back_queue_array(queue_array_t *const queue,
                           const elem_t *const data);

void shift(queue_array_t *const queue);

elem_t pop_front_queue_array(queue_array_t *const queue);

void print_queue_array(const queue_array_t *const queue);

#endif
