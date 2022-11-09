#include "defines.h"
#include "queue_array.h"
#include "gen_number.h"
#include "queue_list.h"

int main(void)
{
    int rc = 0;

    queue_list_t queue_list;
    queue_list.queue_list = NULL;
    queue_list.size = 5;

    for (int8_t i = 0; i < queue_list.size; i++)
    {   
        list_elem_t data = {.arrival_time = i, .process_time = 100 * i};

        list_t *node = create_node(data);

        if (!node)
        {
            goto free;
        }

        queue_list.queue_list = push_back(queue_list.queue_list, node);
    }

    print_list(queue_list.queue_list);

    printf("\n");
    print_list(queue_list.queue_list);

    // while (true)
    // {
    //     if (scanf("%hhd", &item) != 1)
    //     {
    //         break;
    //     }

    //     switch (item)
    //     {
    //     case 1:
    //         if (scanf("%hhd", &data.number) != 1)
    //         {
    //             break;
    //         }

    //         push_back_queue_array(&queue_array, &data);

    //         printf("p_in = %d, p_out = %d\n", queue_array.p_in, queue_array.p_out);
    //         print_queue_array(&queue_array);
    //         break;
        
    //     case 2:
    //         pop_front_queue_array(&queue_array);
    //         printf("p_in = %d, p_out = %d\n", queue_array.p_in, queue_array.p_out);
    //         print_queue_array(&queue_array);
    //         break;
    //     case 0:
    //         break;
    //     }
    // }
    

free:
    free_list(queue_list.queue_list);

    return rc;
}