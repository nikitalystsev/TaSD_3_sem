#ifndef __QUEUE_ARRAY_H__
#define __QUEUE_ARRAY_H__

#include "defines.h"

typedef struct queue_array_t queue_array_t;

struct queue_array_t
{
    elem_t *queue_array; // массив заявок
    int p_in;            // индекс очередного добавленного элемента
    int p_out;           // индекс очередного исключающего элемента
    int size;        // размер массива заявок
};

int create_queue_array(queue_array_t *queue);

void push_back_queue_array(queue_array_t *queue, elem_t *data);

void shift(queue_array_t *queue);

elem_t pop_front_queue_array(queue_array_t *queue);

void print_queue_array(queue_array_t *queue);

#endif
