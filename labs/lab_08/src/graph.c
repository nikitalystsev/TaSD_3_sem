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

adj_list_t *push_end(adj_list_t *head, adj_list_t *node)
{
    adj_list_t *cur = head;

    if (!head)
        return node;

    for (; cur->next; cur = cur->next)
        ;

    cur->next = node;

    return head;
}

void default_graph(graph_t *graph)
{
    for (int i = 0; i < graph->size; i++)
    {
        graph->data[i].head = NULL;
        graph->data[i].is_visited = false;
    }
}

/// @brief модифицированный алгоритм поиска в глубину
/// @param graph граф
/// @param number вершина, с которой начинается обход
/// @return количество пройденных вершин
int DFS(graph_t *graph, int number)
{
    int count_visited = 1;

    graph->data[number - 1].is_visited = true;

    adj_list_t *tmp = graph->data[number - 1].head;

    while (tmp)
    {
        int cur_number = tmp->number;

        if (!graph->data[cur_number - 1].is_visited)
            count_visited += DFS(graph, cur_number);

        tmp = tmp->next;
    }

    return count_visited;
}

// основана на формуле степеней графа
int get_count_edges(graph_t *graph)
{
    int sum_degrees = 0;

    for (int i = 0; i < graph->size; i++)
        sum_degrees += graph->data[i].degree;

    int count_edges = sum_degrees / 2;

    return count_edges;
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
