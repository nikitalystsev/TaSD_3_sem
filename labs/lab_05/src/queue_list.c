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
    printf("[+] queue elem: %d, address elem: %p\n",
           node->data.count_iter,
           (void *)node);
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

int create_free_addr(free_addr_t *free_addr)
{
    free_addr->free_addrs = calloc(free_addr->size, sizeof(addr_t));

    if (!free_addr->free_addrs)
    {
        printf(VIOLET "[-] Ошибка выделения памяти!\n" RESET);
        return ERR_ALLOC_MEM;
    }

    free_addr->top = -1;

    return EXIT_SUCCESS;
}

void init_free_addrs(free_addr_t *free_addr)
{
    for (int i = 0; i < free_addr->size; i++)
    {
        free_addr->free_addrs[i].check_create = false;
        free_addr->free_addrs[i].check_free = false;
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
               (void *)free_addr->free_addrs[i].addr);
    }
}
