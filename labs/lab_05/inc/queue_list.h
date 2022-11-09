#ifndef __QUEUE_LIST_H__
#define __QUEUE_LIST_H__

#include "defines.h"

#define ADD(number, head)                   \
    do                                      \
    {                                       \
        node_t *node = create_node(number); \
        if (!node)                          \
        {                                   \
            return NULL;                    \
        }                                   \
        head = push_front(head, node);      \
    } while (0);

typedef struct list_elem_t list_elem_t;
typedef struct list_t list_t;
typedef struct queue_list_t queue_list_t;

// /// @brief элемент очереди - заявка
struct list_elem_t
{
    double arrival_time; // время прихода
    double process_time; // время обработки
};

// struct list_elem_t
// {
//     int8_t number;
// };

/// @brief односвязный список, содержащий данные
struct list_t
{
    list_elem_t data; // данные
    list_t *next;     // указатель на следующий элемент
};

/// @brief очередь в виде односвязного списка
struct queue_list_t
{
    list_t *queue_list; // очередь в виде списка
    int32_t size;       // количество злементов в списке
};

list_t *create_node(list_elem_t data);

list_t *push_back(list_t *head, list_t *node);

list_t *pop_front(list_t *head);

void free_node(list_t *node);

void free_list(list_t *head);

void print_node(list_t *node);

void print_list(list_t *head);

#endif
