#include "queue_list.h"

list_t *create_node(const elem_t data)
{
    list_t *node = malloc(sizeof(list_t));

    if (node)
    {
        node->data = data;
        node->next = NULL;
    }

    return node;
}

int check_node(const list_t *const node)
{
    if (!node)
    {
        printf(VIOLET "[-] Ошибка выделения памяти!\n" RESET);
        return ERR_ALLOC_MEM;
    }

    return EXIT_SUCCESS;
}

list_t *push_back_queue_list(list_t *const head, list_t *node)
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

list_t *pop_front_queue_list(list_t *const head, elem_t *const list_elem)
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

void free_node(list_t *const node)
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

void print_node(const list_t *const node)
{
    printf("[+] count iter: %d\n", node->data.count_iter);
    // printf("[+] arrival time: %lf\n", node->data.arrival_time);
    // printf("[+] processing time: %lf\n", node->data.processing_time);
}

void print_queue_list(list_t *const head)
{
    if (!head)
    {
        puts(VIOLET "[+] Очередь как список пустая!" RESET);
        return;
    }

    list_t *tmp_head = head;

    while (tmp_head)
    {
        print_node(tmp_head);
        tmp_head = tmp_head->next;
    }
}

void print_free_address(const free_addr_t *const free_addr)
{
    if (free_addr->top == -1)
    {
        printf(VIOLET "\n[-] Массив свободных адресов пустой!\n" RESET);
        return;
    }

    printf("\nМассив ранее высвобожденных адресов:\n\n");

    for (int i = 0; i <= free_addr->top; i++)
    {
        printf("[+] free address: %p\n",
               (void *)free_addr->array_free_addr[i]);
    }
}
