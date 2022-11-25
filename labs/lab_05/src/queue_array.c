#include "queue_array.h"

int create_queue_array(queue_array_t *const queue)
{
    queue->queue_array = calloc(queue->size, sizeof(elem_t));

    if (!queue->queue_array)
    {
        printf(VIOLET "[-] Ошибка выделения памяти!\n" RESET);
        return ERR_ALLOC_MEM;
    }

    queue->p_in = -1;
    queue->p_out = 0;

    return EXIT_SUCCESS;
}

void shift(queue_array_t *const queue)
{
    for (int i = 0; i < queue->p_in; i++)
    {
        queue->queue_array[i] = queue->queue_array[i + 1];
    }

    queue->p_in--;
}

void push_back_queue_array(queue_array_t *const queue,
                           const elem_t *const data)
{
    if (queue->p_in >= queue->size - 1)
    {
        printf(VIOLET "[-] Очередь как массив заполнена максимально! "
                      "Добавление невозможно.\n" RESET);
        return;
    }

    queue->queue_array[++queue->p_in] = *data;
}

elem_t pop_front_queue_array(queue_array_t *const queue)
{
    elem_t elem = queue->queue_array[queue->p_out];

    if (queue->p_in == -1)
    {
        printf(VIOLET "[-] Очередь как массив пустая! "
                      "Удаление невозможно.\n" RESET);
    }
    else
    {
        shift(queue);
    }

    return elem;
}

void print_queue_array(const queue_array_t *const queue)
{
    if (queue->p_in == -1)
    {
        puts(VIOLET "[+] Очередь как массив пустая!" RESET);
        return;
    }

    printf("array:\n");
    for (int8_t i = queue->p_out; i <= queue->p_in; i++)
    {
        printf("[+] count iter: %d\n", queue->queue_array[i].count_iter);
        // printf("[+] arrival time: %lf\n",
        //        queue_array->queue_array[i].arrival_time);
        // printf("[+] processing time: %lf\n",
        //        queue_array->queue_array[i].processing_time);
    }
}
