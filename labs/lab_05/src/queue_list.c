#include "queue_list.h"
#include "read.h"

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
        puts(VIOLET "[-] Ошибка выделения памяти!" RESET);
        return ERR_ALLOC_MEM;
    }

    return EXIT_SUCCESS;
}

list_t *push_queue_list(list_t *const head, list_t *node)
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

list_t *pop_queue_list(list_t *const head, elem_t *const list_elem)
{
    if (!head)
    {
        puts(VIOLET "[-] Очередь как список пустая! "
                    "Удалять нечего." RESET);
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
        if (free_addr->free_addrs[i].check_create)
            printf("Созданный адрес: %p\n",
                   (void *)free_addr->free_addrs[i].addr);
        if (free_addr->free_addrs[i].check_free)
            printf("Освобожденный адрес адрес: %p\n",
                   (void *)free_addr->free_addrs[i].addr);
    }
}

void make_queue_list(queue_list_t *const queue, int *const check)
{
    if (!queue->queue)
    {
        puts(GREEN "[+] Очередь как список была успешно создана!" RESET);
        *check = 1;
    }
    else
    {
        puts(VIOLET "[+] Очередь как список была создана ранее!" RESET);
    }
}

int add_elem_in_list(queue_list_t *const queue, int *const check,
                     elem_t *const elem)
{
    int rc = 0;

    if (*check)
    {
        if ((rc = read_queue_elem(&elem->count_iter)) != 0)
            return rc;

        list_t *node = create_node(*elem);
        if ((rc = check_node(node)) != 0)
            return rc;

        queue->queue = push_queue_list(queue->queue, node);
        queue->size++;

        puts(GREEN "[+] Элемент был успешно добавлен в хвост очереди!" RESET);
    }
    else
    {
        puts(VIOLET "[-] Очередь как список еще не создана!" RESET);
    }

    return rc;
}

void del_elem_from_list(queue_list_t *const queue, free_addr_t *const addrs,
                        elem_t *const elem, int *const check)
{
    if (*check)
    {
        if (queue->queue)
        {
            addrs->free_addrs[++addrs->top].addr = (size_t *)queue->queue;
            
            queue->queue = pop_queue_list(queue->queue, elem);
            queue->size--;

            printf("size = %d\n", queue->size);

            if (queue->size >= 0)
            {
                puts(GREEN "[+] Элемент был успешно "
                           "удален из головы очереди!" RESET);
            }
        }
        else
        {
            puts(VIOLET "[-] Очередь как список пустая!" RESET);
        }
    }
    else
    {
        puts(VIOLET "[-] Очередь как список еще не создана!" RESET);
    }
}

void print_list(queue_list_t *const queue, int *const check)
{
    if (*check)
    {
        print_queue_list(queue->queue);
    }
    else
    {
        puts(VIOLET "[-] Очередь как список не создана!" RESET);
    }
}

void free_queue_list(queue_list_t *const queue, int *const check)
{
    if (*check)
    {
        free_list(queue->queue);
        *check = 0;
        puts(GREEN "[+] Очередь как список была успешно очищена!" RESET);
    }
    else
    {
        puts(VIOLET "[+] Очередь как список не была создана!" RESET);
    }
}