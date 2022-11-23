#include "process.h"
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
    queue.size = 300000;

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

    int count_len_queue_between_print = 0;
    double sum_curr_len = 0;

    double temp = 0, temp2 = 0;

    int flag = 0;

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
                push_back_queue_array(&queue, &new_elem);
                flag = 0;
                model.count_input++;
            }
        }

        sum_curr_len += (queue.p_in - queue.p_out + 1);
        count_len_queue_between_print++;

        if (model.count_output == i)
        {
            model.av_len_queue = sum_curr_len / count_len_queue_between_print;
            model.curr_len_queue = queue.p_in - queue.p_out + 1;
            print_intermediate(&model);
            sum_curr_len = 0;
            count_len_queue_between_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    model.downtime = model.input_time - model.output_time;

    model.model_time =
        (model.input_time > model.output_time)
            ? model.input_time
            : model.output_time;

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

    int count_len_queue_between_print = 0;
    double sum_curr_len = 0;

    double temp = 0, temp2 = 0;

    int flag = 0;

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
                // default_new_application_list(&new_elem, param);
                temp -= temp2;
                model.input_time += new_elem.add_time;
                model.output_time += new_elem.process_time;

                list_t *node = create_node(new_elem);

                if ((rc = check_node(node)) != 0)
                    goto free;

                queue.queue_list =
                    push_back_queue_list(queue.queue_list, node);
                flag = 0;
                queue.size++;
                model.count_input++;
            }
        }

        sum_curr_len += queue.size;
        count_len_queue_between_print++;

        if (model.count_output == i)
        {
            model.av_len_queue = sum_curr_len / count_len_queue_between_print;
            model.curr_len_queue = queue.size;
            print_intermediate(&model);
            sum_curr_len = 0;
            count_len_queue_between_print = 0;
            i += COUNT_PRINT_APPLICATION;
        }
    }

    model.downtime = model.input_time - model.output_time;

    model.model_time =
        (model.input_time > model.output_time)
            ? model.input_time
            : model.output_time;

    print_result(&model, param);

free:
    free_list(queue.queue_list);

    return rc;
}

int read_work_num(int *const number)
{
    printf(TURQ "Введите значение 0,"
                " если хотите решить задачу моделирования СМО "
                ", или введите значение 1, "
                "если хотите работать с очередью в ручном режиме:\n" RESET);
    if (scanf("%d", number) != 1)
    {
        printf(RED "[-] Некорректный ввод значения! "
                   "Выбор - целое число от 0 до 1\n" RESET);
        return ERR_READ_WORK;
    }

    if (*number < 0 || *number > 1)
    {
        printf(RED "[-] Неверный ввод значения! "
                   "Выбор - целое число от 0 до 1\n" RESET);
        return ERR_WORK;
    }

    return EXIT_SUCCESS;
}

static int read_task_menu_item(int8_t *const item)
{
    if (scanf("%hhd", item) != 1)
    {
        printf(RED "[-] Некорректный ввод пункта меню! "
                   "Пункт меню - целое число от 0 до 4\n" RESET);
        return ERR_READ_ITEM;
    }

    if (*item < 0 || *item > 4)
    {
        printf(RED "[-] Неверный ввод пункта меню! "
                   "Пункт меню - целое число от 0 до 4\n" RESET);
        return ERR_ITEM;
    }

    return EXIT_SUCCESS;
}

static int read_range_time(double *const min,
                           double *const max)
{
    puts("\nВведите минимальное значение времени:");
    if (scanf("%lf", min) != 1)
    {
        printf(RED "[-] Некорректный ввод минимального времени!\n" RESET);
        return ERR_READ_TIME;
    }

    puts("Введите максимальное значение времени:");
    if (scanf("%lf", max) != 1)
    {
        printf(RED "[-] Некорректный ввод максимального времени!\n" RESET);
        return ERR_READ_TIME;
    }

    if (*min >= *max)
    {
        printf(RED "[-] Неверный ввод! "
                   "Минимум не дожен быть больше максимума\n" RESET);
        return ERR_TIME;
    }

    if (*min < 0 || *max < 0)
    {
        printf(RED "[-] Неверный ввод! "
                   "Значения не могут быть отрицательными!\n" RESET);
        return ERR_TIME;
    }

    return EXIT_SUCCESS;
}

static int change_time(double *const min,
                       double *const max)
{
    double tmp_min, tmp_max;

    int rc = 0;

    if ((rc = read_range_time(&tmp_min, &tmp_max)) != 0)
        return rc;

    *min = tmp_min;
    *max = tmp_max;

    return EXIT_SUCCESS;
}

int do_modeling(void)
{
    int rc = 0;

    param_t param;
    fill_default_param(&param);

    int8_t menu_item;

    while (true)
    {
        print_task_menu(&param);

        if ((rc = read_task_menu_item(&menu_item)) != 0)
            return rc;

        switch (menu_item)
        {
        case 0:
            exit(0);
            break;
        case 1:
            if ((rc = change_time(&param.min_process_time,
                                  &param.max_process_time)) != 0)
                return rc;
            break;
        case 2:
            if ((rc = change_time(&param.min_add_time,
                                  &param.max_add_time)) != 0)
                return rc;
            break;
        case 3:
            if ((rc = model_list(&param)) != 0)
                return rc;
            break;
        case 4:
            if ((rc = model_arr(&param)) != 0)
                return rc;
            break;
        }
    }

    return rc;
}

static int read_menu_item(int8_t *const item)
{
    if (scanf("%hhd", item) != 1)
    {
        printf(RED "[-] Некорректный ввод пункта меню! "
                   "Пункт меню - целое число от 0 до 11\n" RESET);
        return ERR_READ_ITEM;
    }

    if (*item < 0 || *item > 11)
    {
        printf(RED "[-] Неверный ввод пункта меню! "
                   "Пункт меню - целое число от 0 до 11\n" RESET);
        return ERR_ITEM;
    }

    return EXIT_SUCCESS;
}

static int read_size_queue_array(int *const size)
{
    puts("Введите размер очереди как массива (целое число от 1 до 10000):");
    if (scanf("%d", size) != 1)
    {
        printf(RED "[-] Некорректный ввод размера очереди!\n" RESET);
        return ERR_READ_SIZE;
    }

    if (*size < 1 || *size > MAX_SIZE_QUEUE)
    {
        printf(RED "[-] Неверный ввод размера очереди! "
                   "Размер очереди - целое число от 1 до 10000\n" RESET);
        return ERR_SIZE;
    }

    return EXIT_SUCCESS;
}

static int read_queue_elem(int *const num)
{
    puts("Введите элемент очереди - целое число:");
    if (scanf("%d", num) != 1)
    {
        printf(RED "[-] Некорректный ввод элемента очереди!\n" RESET);
        return ERR_QUEUE_ELEM;
    }

    return EXIT_SUCCESS;
}

int do_action(void)
{
    int rc = 0;

    queue_array_t queue_array;
    queue_array.queue_array = NULL;

    queue_list_t queue_list;
    queue_list.size = 0;
    queue_list.queue_list = NULL;

    int8_t menu_item;

    elem_t arr_elem, list_elem;

    free_addr_t free_addr;
    free_addr.top = -1;
    int check_queue_list = 0;

    while (true)
    {
        print_menu();

        if ((rc = read_menu_item(&menu_item)) != 0)
            return rc;

        switch (menu_item)
        {
        case 0:
            exit(0);
            break;
        case 1:
            if (!queue_array.queue_array)
            {
                if ((rc = read_size_queue_array(&queue_array.size)) != 0)
                    goto free;
                if ((rc = create_queue_array(&queue_array)) != 0)
                    goto free;
            }
            else
                puts(VIOLET "[+] "
                            "Очередь как массив была создана ранее!" RESET);
            break;
        case 2:
            if (queue_array.queue_array)
            {
                if ((rc = read_queue_elem(&arr_elem.count_iter)) != 0)
                    goto free;
                push_back_queue_array(&queue_array, &arr_elem);
                if (queue_array.p_in != queue_array.size - 1)
                    puts(GREEN "[+] Элемент был успешно добавлен "
                               "в хвост очереди!" RESET);
            }
            else
                puts(VIOLET "[-] Очередь как массив не создана!" RESET);
            break;
        case 3:
            if (queue_array.queue_array)
            {
                pop_front_queue_array(&queue_array);
                if (queue_array.p_in != -1)
                    puts(GREEN "[+] Элемент был успешно удален "
                               "из головы очереди!" RESET);
            }
            else
                puts(VIOLET "[-] Очередь как массив не создана!" RESET);
            break;
        case 4:
            if (queue_array.queue_array)
                print_queue_array(&queue_array);
            else
                puts(VIOLET "[-] Очередь как массив не создана!" RESET);
            break;
        case 5:
            free(queue_array.queue_array);
            queue_array.queue_array = NULL;
            puts(GREEN "[+] "
                       "Очередь как массив была успешно очищена!" RESET);
            break;
        case 6:
            if (!queue_list.queue_list)
            {
                puts(GREEN "[+] "
                           "Очередь как список была успешно создана!" RESET);
                check_queue_list = 1;
            }
            else
                puts(VIOLET "[+] "
                            "Очередь как список была создана ранее!" RESET);
            break;
        case 7:
            if (check_queue_list)
            {
                if ((rc = read_queue_elem(&list_elem.count_iter)) != 0)
                    goto free;
                list_t *node = create_node(list_elem);
                queue_list.queue_list =
                    push_back_queue_list(queue_list.queue_list, node);
                puts(GREEN "[+] Элемент был успешно добавлен "
                           "в хвост очереди!" RESET);
            }
            else
                puts(VIOLET "[-] Очередь как список еще не создана!" RESET);
            break;
        case 8:
            if (check_queue_list)
            {
                free_addr.array_free_addr[++free_addr.top] =
                    (size_t *)queue_list.queue_list;
                if ((queue_list.queue_list = pop_front_queue_list(
                         queue_list.queue_list, &list_elem)) != NULL)
                    puts(GREEN "[+] Элемент был успешно удален "
                               "из головы очереди!" RESET);
            }
            else
                puts(VIOLET "[-] Очередь как список еще не создана!" RESET);
            break;
        case 9:
            if (check_queue_list)
                print_queue_list(queue_list.queue_list);
            else
                puts(VIOLET "[-] Очередь как список не создана!" RESET);
            break;
        case 10:
            free_list(queue_list.queue_list);
            queue_list.queue_list = NULL;
            check_queue_list = 0;
            puts(GREEN "[+] "
                       "Очередь как список была успешно очищена!" RESET);
            break;
        case 11:
            print_free_address(&free_addr);
            break;
        }
    }

free:
    free(queue_array.queue_array);
    free_list(queue_list.queue_list);

    return rc;
}
