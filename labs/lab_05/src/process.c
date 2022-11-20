#include "process.h"
#include "print.h"

void fill_default_parametres(parametres_t *const parametres)
{
    parametres->min_time_add = MIN_TIME_ADD;
    parametres->max_time_add = MAX_TIME_ADD;
    parametres->min_time_processing = MIN_TIME_PROCESSING;
    parametres->max_time_processing = MAX_TIME_PROCESSING;
    parametres->count_application = COUNT_APPLICATION;
    parametres->count_print_application = COUNT_PRINT_APPLICATION;
}

static void fill_default_data_modeling(data_modeling_t *const data_modeling)
{
    data_modeling->current_length_queue = 0;
    data_modeling->average_length_queue = 0;
    data_modeling->count_calls_machine = 0;
    data_modeling->downtime_machine = 0;
    data_modeling->total_input_application = 0;
    data_modeling->total_output_application = 0;
    data_modeling->total_simulation_time = 0;
    data_modeling->time_input_all_application = 0;
    data_modeling->time_output_all_application = 0;
}

static void default_new_application_arr(arr_elem_t *const arr_elem,
                                        const parametres_t *const parametres)
{
    arr_elem->arrival_time = random_double(parametres->min_time_add,
                                           parametres->max_time_add);
    arr_elem->processing_time = random_double(parametres->min_time_processing,
                                              parametres->max_time_processing);
    arr_elem->count_iter = 0;
}

static void default_new_application_list(list_elem_t *const list_elem,
                                         const parametres_t *const parametres)
{
    list_elem->arrival_time = random_double(parametres->min_time_add,
                                            parametres->max_time_add);
    list_elem->processing_time = random_double(parametres->min_time_processing,
                                               parametres->max_time_processing);
    list_elem->count_iter = 0;
}

int modeling_process_arr(const parametres_t *const parametres)
{
    int rc = 0;

    srand(time(NULL));

    queue_array_t queue;
    queue.size = 10000;

    if ((rc = create_queue_array(&queue)) != 0)
    {
        printf("[-] Ошибка выделения памяти!\n");
        return rc;
    }

    data_modeling_t data_modeling;

    fill_default_data_modeling(&data_modeling);

    bool service_machine = false;

    arr_elem_t temp_elem, new_elem;

    int i = COUNT_PRINT_APPLICATION;

    int count_len_queue_between_print = 0;
    double sum_curr_len = 0;

    while (data_modeling.total_output_application <
           parametres->count_application)
    {
        if (!service_machine)
        {
            /* если аппарат не работает */
            if (queue.p_in == -1)
            {
                /* если в очереди нет элементов */
                default_new_application_arr(&new_elem, parametres);
                push_back_queue_array(&queue, &new_elem);
                data_modeling.total_input_application++;
                data_modeling.total_simulation_time += new_elem.arrival_time;
                data_modeling.time_input_all_application +=
                    new_elem.arrival_time;
                data_modeling.time_output_all_application +=
                    new_elem.processing_time;
            }
            else
            {
                /* если в очереди есть элементы */
                temp_elem = pop_front_queue_array(&queue);
                service_machine = true;
                data_modeling.count_calls_machine++;
            }
        }
        else
        {
            /* если аппарат работает */
            if (new_elem.arrival_time > temp_elem.processing_time)
            {
                /*
                если заявка обработается раньше,
                чем другая заявка придет в очередь,
                то аппарат заканчивает работу
                */
                new_elem.arrival_time -= temp_elem.processing_time;
                service_machine = false;
                temp_elem.count_iter++;

                if (temp_elem.count_iter < COUNT_PROCESS)
                {
                    /*
                    если из аппарата вышла заявка,
                    которая проходила очередь меньше 5 раз,
                    добавляем ее обратно
                    */
                    data_modeling.time_output_all_application +=
                        temp_elem.processing_time;
                    push_back_queue_array(&queue, &temp_elem);
                    data_modeling.total_input_application++;
                }
                else
                {
                    /* иначе покидает систему */
                    data_modeling.total_output_application++;
                }
            }
            else
            {
                /*
                если заявка обработается позже,
                чем другая заявка придет в очередь, то добавляем другую заявку
                */
                default_new_application_arr(&new_elem, parametres);
                // new_elem.processing_time -= temp_elem.arrival_time;
                data_modeling.time_input_all_application +=
                    new_elem.arrival_time;
                data_modeling.time_output_all_application +=
                    new_elem.processing_time;
                push_back_queue_array(&queue, &new_elem);
                data_modeling.total_simulation_time += new_elem.arrival_time;
                data_modeling.total_input_application++;
            }
        }

        sum_curr_len += (queue.p_in - queue.p_out + 1);
        count_len_queue_between_print++;

        if (data_modeling.total_output_application == i)
        {
            data_modeling.average_length_queue =
                sum_curr_len / count_len_queue_between_print;
            data_modeling.current_length_queue = queue.p_in - queue.p_out + 1;
            print_intermediate(&data_modeling);
            sum_curr_len = 0;
            count_len_queue_between_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    data_modeling.downtime_machine = data_modeling.time_input_all_application -
                                     data_modeling.time_output_all_application;

    print_result(&data_modeling, parametres);

    free(queue.queue_array);

    return rc;
}

int modeling_process_list(const parametres_t *const parametres)
{
    int rc = 0;

    srand(time(NULL));

    queue_list_t queue;
    queue.queue_list = NULL;
    queue.size = 0;

    data_modeling_t data_modeling;

    fill_default_data_modeling(&data_modeling);

    bool service_machine = false;

    list_elem_t temp_elem, new_elem;

    int i = COUNT_PRINT_APPLICATION;

    int count_len_queue_between_print = 0;
    double sum_curr_len = 0;

    while (data_modeling.total_output_application <
           parametres->count_application)
    {
        if (!service_machine)
        {
            /* если аппарат не работает */
            if (queue.size == 0)
            {
                /* если в очереди нет элементов */
                default_new_application_list(&new_elem, parametres);
                list_t *node = create_node(new_elem);
                if (!node)
                {
                    rc = ERR_ALLOC_MEM;
                    goto free;
                }
                queue.queue_list =
                    push_back_queue_list(queue.queue_list, node);
                queue.size++;
                data_modeling.total_input_application++;
                data_modeling.total_simulation_time += new_elem.arrival_time;
                data_modeling.time_input_all_application +=
                    new_elem.arrival_time;
                data_modeling.time_output_all_application +=
                    new_elem.processing_time;
            }
            else
            {
                /* если в очереди есть элементы */
                queue.queue_list =
                    pop_front_queue_list(queue.queue_list, &temp_elem);
                queue.size--;
                service_machine = true;
                data_modeling.count_calls_machine++;
            }
        }
        else
        {
            /* если аппарат работает */
            if (new_elem.arrival_time > temp_elem.processing_time)
            {
                /*
                если заявка обработается раньше,
                чем другая заявка придет в очередь,
                то аппарат заканчивает работу
                */
                new_elem.arrival_time -= temp_elem.processing_time;
                service_machine = false;
                temp_elem.count_iter++;

                if (temp_elem.count_iter < COUNT_PROCESS)
                {
                    /*
                    если из аппарата вышла заявка,
                    которая проходила очередь меньше 5 раз,
                    добавляем ее обратно
                    */
                    data_modeling.time_output_all_application +=
                        temp_elem.processing_time;
                    list_t *node = create_node(temp_elem);
                    if (!node)
                    {
                        rc = ERR_ALLOC_MEM;
                        goto free;
                    }
                    queue.queue_list =
                        push_back_queue_list(queue.queue_list, node);
                    queue.size++;
                    data_modeling.total_input_application++;
                }
                else
                {
                    /* иначе покидает систему */
                    data_modeling.total_output_application++;
                }
            }
            else
            {
                /*
                если заявка обработается позже,
                чем другая заявка придет в очередь, то добавляем другую заявку
                */
                default_new_application_list(&new_elem, parametres);
                // new_elem.processing_time -= temp_elem.arrival_time;
                data_modeling.time_input_all_application +=
                    new_elem.arrival_time;
                data_modeling.time_output_all_application +=
                    new_elem.processing_time;
                list_t *node = create_node(new_elem);
                if (!node)
                {
                    rc = ERR_ALLOC_MEM;
                    goto free;
                }
                queue.queue_list =
                    push_back_queue_list(queue.queue_list, node);
                queue.size++;
                data_modeling.total_simulation_time += new_elem.arrival_time;
                data_modeling.total_input_application++;
            }
        }

        sum_curr_len += queue.size;
        count_len_queue_between_print++;

        if (data_modeling.total_output_application == i)
        {
            data_modeling.average_length_queue =
                sum_curr_len / count_len_queue_between_print;
            data_modeling.current_length_queue = queue.size;
            print_intermediate(&data_modeling);
            sum_curr_len = 0;
            count_len_queue_between_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    data_modeling.downtime_machine = data_modeling.time_input_all_application -
                                     data_modeling.time_output_all_application;

    print_result(&data_modeling, parametres);

free:
    free_list(queue.queue_list);

    return rc;
}
