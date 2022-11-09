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

list_t *push_back(list_t *head, list_t *node)
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

list_t *pop_front(list_t *head)
{
    if (!head)
    {
        printf(VIOLET "[-] Очередь как список пустая! "
                      "Удалять нечего.\n" RESET);
        return NULL;
    }

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
    printf("[+] data: %lf, %lf\n",
           node->data.arrival_time,
           node->data.process_time);
}

void print_list(list_t *head)
{
    list_t *tmp_head = head;

    while (tmp_head)
    {
        print_node(tmp_head);
        tmp_head = tmp_head->next;
    }
}
