#include "queue_array.h"

int create_queue_array(queue_array_t *queue_array)
{
    queue_array->queue_array =
        calloc(queue_array->size, sizeof(arr_elem_t));

    if (!queue_array->queue_array)
    {
        printf("[-] Ошибка выделения памяти!\n");
        return ERR_ALLOC_MEM;
    }

    queue_array->p_in = -1;
    queue_array->p_out = 0;

    return EXIT_SUCCESS;
}

void shift(queue_array_t *queue_array)
{
    for (int8_t i = 0; i < queue_array->p_in; i++)
    {
        queue_array->queue_array[i] = queue_array->queue_array[i + 1];
    }

    queue_array->p_in--;
}

void push_back_queue_array(queue_array_t *queue_array,
                           arr_elem_t *data)
{
    if (queue_array->p_in >= queue_array->size - 1)
    {
        printf(VIOLET "[-] Очередь как массив заполнена максимально! "
                      "Добавление невозможно.\n" RESET);
        return;
    }

    queue_array->queue_array[++queue_array->p_in] = *data;
    // queue_array->p_in++;
}

arr_elem_t pop_front_queue_array(queue_array_t *queue_array)
{
    arr_elem_t elem = queue_array->queue_array[queue_array->p_out];

    if (queue_array->p_in == -1)
    {
        printf(VIOLET "[-] Очередь как массив пустая! "
                      "Удаление невозможно.\n" RESET);
    }
    else
    {
        shift(queue_array);
    }

    return elem;
}

void print_queue_array(queue_array_t *queue_array)
{
    printf("array:\n");

    for (int8_t i = queue_array->p_out; i <= queue_array->p_in; i++)
    {
        printf("[+] count iter: %d\n", queue_array->queue_array[i].count_iter);
        printf("[+] arrival time: %lf\n",
               queue_array->queue_array[i].arrival_time);
        printf("[+] processing time: %lf\n",
               queue_array->queue_array[i].processing_time);
    }
}
