#ifndef __QUEUE_LIST_H__
#define __QUEUE_LIST_H__

#include "defines.h"

typedef struct list_t list_t;
typedef struct queue_list_t queue_list_t;
typedef struct free_addr_t free_addr_t;

/// @brief односвязный список, содержащий данные
struct list_t
{
    elem_t data;  // данные
    list_t *next; // указатель на следующий элемент
};

/// @brief очередь в виде односвязного списка
struct queue_list_t
{
    list_t *queue_list; // очередь в виде списка
    int size;           // количество злементов в списке
};

/// @brief массив свободных адресов
struct free_addr_t
{
    // массив ранее высвобожденных адресов
    size_t *array_free_addr[MAX_FREE_ADDR_SIZE];
    int top; // индекс последнего  добавленного элемента
};

list_t *create_node(const elem_t data);

int check_node(const list_t *const node);

list_t *push_back_queue_list(list_t *const head, list_t *node);

list_t *pop_front_queue_list(list_t *const head, elem_t *const list_elem);

void free_node(list_t *const node);

void free_list(list_t *head);

void print_node(const list_t *const node);

void print_queue_list(list_t *const head);

void print_free_address(const free_addr_t *const free_addr);

#endif
