#include "process.h"
#include "print.h"

double random_double(double min, double max)
{
    double x = rand() / (double)RAND_MAX; /* [0, 1.0] */

    return min + x * (max - min);
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

static int read_range_time(double *const min, double *const max)
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

static int change_time(double *const min, double *const max)
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
