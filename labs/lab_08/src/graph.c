#include "graph.h"

adj_list_t *create_node(int number)
{
    adj_list_t *node = malloc(sizeof(adj_list_t));

    if (node)
    {
        node->number = number;
        node->next = NULL;
    }

    return node;
}

int create_graph(graph_t *graph)
{
    graph->data = malloc(sizeof(vertex_t) * graph->size);

    if (!graph->data)
        return ERR_MEM_ALLOC;

    return EXIT_SUCCESS;
}

void free_agj_list(adj_list_t *head)
{
    adj_list_t *next;

    for (; head; head = next)
    {
        next = head->next;
        free(head);
    }
}

void free_graph(graph_t *graph)
{
    for (int i = 0; i < graph->size; i++)
        free_agj_list(graph->data[i].head);
    
    free(graph->data);
}