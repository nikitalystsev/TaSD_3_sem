#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "defines.h"

typedef struct graph_t graph_t;
typedef struct vertex_t vertex_t;
typedef struct adj_list_t adj_list_t;

struct adj_list_t
{
    int number;
    adj_list_t *next;
};
struct vertex_t
{
    int number;
    int degree;

};

struct graph_t
{
    int number;
};

#endif
