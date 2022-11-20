#include "defines.h"
#include "queue_array.h"
#include "gen_number.h"
#include "queue_list.h"
#include "process.h"

int main(void)
{
    int rc = 0;

    // queue_array_t queue;
    // queue.size = 5;
    // create_queue_array(&queue);

    // arr_elem_t data;

    // int i = 0;

    // while (true)
    // {
    //     int num;
    //     rc = scanf("%d", &num);
    //     if (num == 0)
    //         break;
            
    //     switch (num)
    //     {
    //     case 1:
    //         data.count_iter = i;
    //         push_back_queue_array(&queue, &data);
    //         break;
    //     case 2:
    //         pop_front_queue_array(&queue);
    //     default:
    //         break;
    //     }

    //     i++;
    //     print_queue_array(&queue);
    // }

    
    parametres_t param;
    fill_default_parametres(&param);

    rc = modeling_process_list(&param);

    // srand(time(NULL));

    // double sum = 0;

    // for (int i = 0; i < 1000; i++)
    // {
    //     double d = random_double(0, 6);
    //     sum += d;
    //     printf("[+] sum = %lf, d = %lf\n", sum, d);
    // }

    // printf("[+] sum = %lf\n", sum);

    return rc;
}
