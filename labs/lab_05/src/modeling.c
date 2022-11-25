#include "modeling.h"
#include "print.h"

void fill_default_param(param_t *const param)
{
    param->min_add_time = MIN_TIME_ADD;
    param->max_add_time = MAX_TIME_ADD;
    param->min_process_time = MIN_TIME_PROCESSING;
    param->max_process_time = MAX_TIME_PROCESSING;
    param->count_appl = COUNT_APPLICATION;
    param->count_print_appl = COUNT_PRINT_APPLICATION;
}

static void fill_default_model(model_t *const data)
{
    data->curr_len_queue = 0;
    data->av_len_queue = 0;
    data->count_calls_machine = 0;
    data->downtime = 0;
    data->count_input = 0;
    data->count_output = 0;
    data->model_time = 0;
    data->input_time = 0;
    data->output_time = 0;
    data->work_time = 0;
    data->memory_size = 0;
}

static void create_new_appl(elem_t *const elem, const param_t *const param)
{
    elem->add_time = random_double(param->min_add_time,
                                   param->max_add_time);
    elem->process_time = random_double(param->min_process_time,
                                       param->max_process_time);
    elem->count_iter = 0;
}

int model_arr(const param_t *const param)
{
    int rc = 0;

    srand(time(NULL));

    queue_array_t queue;
    queue.size = MAX_SIZE_QUEUE / 10;

    if ((rc = create_queue_array(&queue)) != 0)
    {
        printf(VIOLET "[-] Ошибка выделения памяти!\n" RESET);
        return rc;
    }

    model_t model;

    fill_default_model(&model);

    bool machine = false;

    elem_t temp_elem, new_elem;

    int i = COUNT_PRINT_APPLICATION;

    int count_len_beetw_print = 0;
    double sum_curr_len = 0;

    double temp = 0, temp2 = 0;

    int flag = 0;

    int mem_size = 0;

    model.work_time = microseconds_now();

    while (model.count_output < param->count_appl)
    {
        // printf("в цикле\n");
        if (!machine)
        {
            /* если аппарат не работает */
            // printf("аппарат не работает\n");
            if (queue.p_in == -1)
            {
                /* если в очереди нет элементов */
                if (!flag)
                {
                    create_new_appl(&new_elem, param);
                    flag = 1;
                }

                push_back_queue_array(&queue, &new_elem);

                mem_size = queue_arr_size_in_bytes(&queue);
                model.memory_size = max_int(model.memory_size, mem_size);

                flag = 0;
                model.count_input++;
                model.input_time += new_elem.add_time;
                model.output_time += new_elem.process_time;
            }
            else
            {
                /* если в очереди есть элементы */
                temp_elem = pop_front_queue_array(&queue);
                machine = true;
                model.count_calls_machine++;
                temp = temp_elem.process_time;
            }
        }
        else
        {
            /* если аппарат работает */
            // printf("аппарат работает\n");
            if (!flag)
            {
                create_new_appl(&new_elem, param);
                temp2 = new_elem.add_time;
                flag = 1;
            }

            if (temp2 > temp)
            {
                /*
                если заявка обработается раньше,
                чем другая заявка придет в очередь,
                то аппарат заканчивает работу
                */
                // temp2 -= temp;
                machine = false;
                temp_elem.count_iter++;

                if (temp_elem.count_iter < COUNT_PROCESS)
                {
                    /*
                    если из аппарата вышла заявк
                    которая проходила очередь меньше 5 раз,
                    добавляем ее обратно
                    */
                    model.output_time += temp_elem.process_time;
                    push_back_queue_array(&queue, &temp_elem);

                    mem_size = queue_arr_size_in_bytes(&queue);
                    model.memory_size = max_int(model.memory_size, mem_size);

                    model.count_input++;
                }
                else
                {
                    /* иначе покидает систему */
                    model.count_output++;
                }
            }
            else
            {
                /*
                если заявка обработается позже,
                чем другая заявка придет в очередь,
                то добавляем другую заявку
                */
                temp -= temp2;
                model.input_time += new_elem.add_time;
                model.output_time += new_elem.process_time;
                push_back_queue_array(&queue, &new_elem);

                mem_size = queue_arr_size_in_bytes(&queue);
                model.memory_size = max_int(model.memory_size, mem_size);

                flag = 0;
                model.count_input++;
            }
        }

        sum_curr_len += (queue.p_in - queue.p_out + 1);
        count_len_beetw_print++;

        if (model.count_output == i)
        {
            model.av_len_queue = sum_curr_len / count_len_beetw_print;
            model.curr_len_queue = queue.p_in - queue.p_out + 1;
            print_intermediate(&model);
            sum_curr_len = 0;
            count_len_beetw_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    model.work_time = microseconds_now() - model.work_time;

    model.downtime = model.input_time - model.output_time;

    model.model_time = max_double(model.input_time, model.output_time);

    print_result(&model, param);

    free(queue.queue_array);

    return rc;
}

int model_list(const param_t *const param)
{
    int rc = 0;

    srand(time(NULL));

    queue_list_t queue;
    queue.queue_list = NULL;
    queue.size = 0;

    model_t model;

    fill_default_model(&model);

    bool machine = false;

    elem_t temp_elem, new_elem;

    int i = COUNT_PRINT_APPLICATION;

    int count_len_beetw_print = 0;
    double sum_curr_len = 0;

    double temp = 0, temp2 = 0;

    int flag = 0;

    model.work_time = microseconds_now();

    int mem_size = 0;

    while (model.count_output < param->count_appl)
    {
        if (!machine)
        {
            /* если аппарат не работает */
            if (queue.size == 0)
            {
                /* если в очереди нет элементов */
                if (!flag)
                {
                    create_new_appl(&new_elem, param);
                    flag = 1;
                }

                list_t *node = create_node(new_elem);

                if ((rc = check_node(node)) != 0)
                    goto free;

                queue.queue_list =
                    push_back_queue_list(queue.queue_list, node);

                mem_size = queue_list_size_in_bytes(&queue);
                model.memory_size = max_int(model.memory_size, mem_size);

                flag = 0;
                queue.size++;
                model.count_input++;
                model.input_time += new_elem.add_time;
                model.output_time += new_elem.process_time;
            }
            else
            {
                /* если в очереди есть элементы */
                queue.queue_list =
                    pop_front_queue_list(queue.queue_list, &temp_elem);
                queue.size--;
                machine = true;
                model.count_calls_machine++;
                temp = temp_elem.process_time;
            }
        }
        else
        {
            /* если аппарат работает */

            if (!flag)
            {
                create_new_appl(&new_elem, param);
                temp2 = new_elem.add_time;
                flag = 1;
            }

            if (temp2 > temp)
            {
                /*
                если заявка обработается раньше,
                чем другая заявка придет в очередь,
                то аппарат заканчивает работу
                */
                // new_elem.arrival_time -= temp_elem.processing_time;
                machine = false;
                temp_elem.count_iter++;

                if (temp_elem.count_iter < COUNT_PROCESS)
                {
                    /*
                    если из аппарата вышла заявка,
                    которая проходила очередь меньше 5 раз,
                    добавляем ее обратно
                    */
                    model.output_time += temp_elem.process_time;
                    list_t *node = create_node(temp_elem);

                    if ((rc = check_node(node)) != 0)
                        goto free;

                    queue.queue_list =
                        push_back_queue_list(queue.queue_list, node);
                    mem_size = queue_list_size_in_bytes(&queue);
                    model.memory_size = max_int(model.memory_size, mem_size);
                    queue.size++;
                    model.count_input++;
                }
                else
                {
                    /* иначе покидает систему */
                    model.count_output++;
                }
            }
            else
            {
                /*
                если заявка обработается позже,
                чем другая заявка придет в очередь, то добавляем другую заявку
                */
                temp -= temp2;
                model.input_time += new_elem.add_time;
                model.output_time += new_elem.process_time;

                list_t *node = create_node(new_elem);

                if ((rc = check_node(node)) != 0)
                    goto free;

                queue.queue_list =
                    push_back_queue_list(queue.queue_list, node);
                mem_size = queue_list_size_in_bytes(&queue);
                model.memory_size = max_int(model.memory_size, mem_size);
                flag = 0;
                queue.size++;
                model.count_input++;
            }
        }

        sum_curr_len += queue.size;
        count_len_beetw_print++;

        if (model.count_output == i)
        {
            model.av_len_queue = sum_curr_len / count_len_beetw_print;
            model.curr_len_queue = queue.size;
            print_intermediate(&model);
            sum_curr_len = 0;
            count_len_beetw_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    model.work_time = microseconds_now() - model.work_time;

    model.downtime = model.input_time - model.output_time;

    model.model_time = max_double(model.input_time, model.output_time);

    print_result(&model, param);

free:
    free_list(queue.queue_list);

    return rc;
}
