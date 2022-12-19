#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "defines.h"

typedef struct graph_t graph_t;
typedef struct vertex_t vertex_t;
typedef struct adj_list_t adj_list_t;

/// @brief список смежности
struct adj_list_t
{
    int number; // номер вершины
    adj_list_t *next;
};

/// @brief описание вершины графа
struct vertex_t
{
    int number;       // номер вершины
    int degree;       // степень вершины
    adj_list_t *head; // список смежности данной вершины
};

/// @brief описание графа
struct graph_t
{
    vertex_t *data; // массив  вершин
    int size;       // размер массива - количество вершин
};

adj_list_t *create_node(int number);

int create_graph(graph_t *graph);

void free_agj_list(adj_list_t *head);

void free_graph(graph_t *graph);

#endif
