#include "queue_list.h"

list_t *create_node(list_elem_t data)
{
    list_t *node = malloc(sizeof(list_t));

    if (node)
    {
        node->data = data;
        node->next = NULL;
    }

    return node;
}

list_t *push_back_queue_list(list_t *head, list_t *node)
{
    list_t *tmp_head = head;

    if (!head)
    {
        return node;
    }

    for (; tmp_head->next; tmp_head = tmp_head->next)
        ;

    tmp_head->next = node;

    return head;
}

list_t *pop_front_queue_list(list_t *head, list_elem_t *list_elem)
{
    if (!head)
    {
        printf(VIOLET "[-] Очередь как список пустая! "
                      "Удалять нечего.\n" RESET);
        return NULL;
    }

    *list_elem = head->data;
    list_t *tmp_head = head->next;

    free(head);

    return tmp_head;
}

void free_node(list_t *node)
{
    free(node);
}

void free_list(list_t *head)
{
    list_t *tmp_head = head;

    for (; head; head = tmp_head)
    {
        tmp_head = head->next;
        free_node(head);
    }
}

void print_node(list_t *node)
{
    printf("[+] count iter: %d\n", node->data.count_iter);
    printf("[+] arrival time: %lf\n", node->data.arrival_time);
    printf("[+] processing time: %lf\n", node->data.processing_time);
}

void print_queue_list(list_t *head)
{
    list_t *tmp_head = head;

    while (tmp_head)
    {
        print_node(tmp_head);
        tmp_head = tmp_head->next;
    }
}
