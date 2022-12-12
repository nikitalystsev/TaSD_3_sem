#include "process.h"

static int read_file_name(char *file_name)
{
    puts(TURQ "\nВведите имя файла:" RESET);

    if (scanf("%s", file_name) != 1)
    {
        puts(RED "\nНевалидный ввод имени файла!" RESET);
        return ERR_READ_DATA;
    }

    return EXIT_SUCCESS;
}

static int read_menu_item(int *item)
{
    if (scanf("%d", item) != 1)
    {
        puts(RED "\nНекорректный ввод пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    if (*item < 0 || *item > 9)
    {
        puts(RED "\nНеверный номер пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}

static int read_count_data(int *count_data)
{
    puts("\nВведите количество генерируемых данных "
         "(целое положительное число):");
    if (scanf("%d", count_data) != 1)
    {
        puts(RED "\nНекорректный ввод количества генерируемых данных!" RESET);
        return ERR_COUNT_DATA;
    }

    if (*count_data < 0)
    {
        puts(RED "\nНеверный ввод! "
                 "Количество генерируемых данных - число больше нуля!" RESET);
        return ERR_COUNT_DATA;
    }

    return EXIT_SUCCESS;
}

static bool is_exist_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (!file)
        return false;

    fclose(file);

    return true;
}

static int read_numbers(const char *file_name, tree_t *tree, bool is_balance)
{
    int rc = 0;

    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        puts(RED "\nОшибка открытия файла!\n" RESET);
        return ERR_OPEN_FILE;
    }

    int count, data;

    if (fscanf(file, "%d", &count) != 1)
    {
        fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
        return ERR_READ_DATA;
    }

    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &data) != 1)
        {
            fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
            return ERR_READ_DATA;
        }

        vertex_t *vertex = create_vertex(data, 0);
        if (!vertex)
        {
            fclose(file);
            return ERR_READ_DATA;
        }

        if (is_balance)
            tree->root = add_vertex(tree->root, vertex, true);
        else
            tree->root = add_vertex(tree->root, vertex, false);
    }

    fclose(file);

    return rc;
}

static int read_data(int *count_data, tree_t *tree, tree_t *balance_tree)
{
    free_tree(tree);
    free_tree(balance_tree);

    char file_name[MAX_STR_SIZE];
    char data_file[MAX_STR_SIZE] = DATA_DIR;
    char data_gv[MAX_STR_SIZE];

    int rc = 0;

    if ((rc = read_file_name(file_name)) != 0)
        return rc;

    strcat(data_file, file_name);

    if (!is_exist_file(data_file))
    {
        if ((rc = read_count_data(count_data)) != 0)
            return rc;

        gen_data_file(data_file, *count_data);
    }

    strcpy(data_gv, data_file);
    strcat(data_gv, GV);

    if ((rc = read_numbers(data_file, tree, false)) != 0)
        return rc;

    if ((rc = read_numbers(data_file, balance_tree, true)) != 0)
        return rc;

    if (rc == 0)
        puts(GREEN "\nДанные были успешно прочитаны!" RESET);

    return rc;
}

static int read_elem_tree(int *num)
{
    puts(TURQ "\nВведите элемент дерева:" RESET);

    if (scanf("%d", num) != 1)
    {
        puts(RED "\nНекорректный ввод элемента дерева!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}

static int read_find_elem(int *num)
{
    puts(TURQ "\nВведите элемент дерева, который требуется найти:" RESET);

    if (scanf("%d", num) != 1)
    {
        puts(RED "\nНекорректный ввод элемента дерева!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}

static int add_number(tree_t *tree, int data, int height, bool is_balance)
{
    int rc = 0;

    vertex_t *vertex = create_vertex(data, height);

    if (!vertex)
    {
        puts(RED "Ошибка добавления данных!\n" RESET);
        return ERR_READ_DATA;
    }

    if (is_balance)
        tree->root = add_vertex(tree->root, vertex, true);
    else
        tree->root = add_vertex(tree->root, vertex, false);

    return rc;
}

static int add_data(tree_t *tree, tree_t *balance_tree)
{
    int rc = 0;

    int number, height = 0;

    if ((rc = read_elem_tree(&number)) != 0)
        return rc;

    if ((rc = add_number(tree, number, height, false)) != 0)
        return rc;

    if ((rc = add_number(balance_tree, number, height, true)) != 0)
        return rc;

    if (rc == 0)
        puts(GREEN "\nДанные были успешно добавлены!" RESET);

    return rc;
}

static int del_data(tree_t *tree, tree_t *balance_tree)
{
    int rc = 0;

    int number;

    if ((rc = read_elem_tree(&number)) != 0)
        return rc;

    rc = delete_vertex(&tree->root, number, false);
    rc += delete_vertex(&balance_tree->root, number, true);

    if (rc == 0)
        printf(GREEN "\nЭлемент со значением (%d) "
                     "был успешно удален!\n" RESET,
               number);
    else
        puts(VIOLET "\nУдаляемого элемента в дереве нет" RESET);

    return rc;
}

static int trees_to_dot(tree_t *tree, tree_t *balance_tree)
{
    int rc = 0;

    if (is_empty_tree(tree) || is_empty_tree(balance_tree))
        puts(VIOLET "\nДерево пустое" RESET);
    else
    {
        char data_file[MAX_STR_SIZE] = DATA_DIR;
        char balance_data_file[MAX_STR_SIZE] = DATA_DIR "balance_";

        char data_gv[MAX_STR_SIZE];

        if ((rc = read_file_name(data_gv)) != 0)
            return rc;

        strcat(data_file, data_gv);
        strcat(balance_data_file, data_gv);

        if ((rc = export_to_dot(data_file, "my_tree", tree)) != 0)
            return rc;

        if ((rc = export_to_dot(balance_data_file, "my_tree",
                                balance_tree)) != 0)
            return rc;
    }

    return rc;
}

static bool find_number(tree_t *tree, int number, int *count_compare)
{
    vertex_t *found_elem = search(tree->root, number, count_compare);

    if (found_elem)
        return true;

    return false;
}

static int tree_size(tree_t *tree)
{
    int count_vertex = get_count_vertex(tree->root);

    int size = 0;

    size += sizeof(int);
    size += sizeof(int);
    size += sizeof(vertex_t *);
    size += sizeof(vertex_t *);

    size *= count_vertex;

    return size;
}

static int find_data(tree_t *tree, tree_t *balance_tree)
{
    int rc = 0;

    int number, count_cmp = 0, count_cmp_balance = 0;

    if ((rc = read_find_elem(&number)) != 0)
        return rc;

    bool find, find_balance;
    long double beg, end1 = 0, end2 = 0;

    for (int i = 0; i < N_REPS; i++)
    {
        beg = microseconds_now();
        find = find_number(tree, number, &count_cmp);
        end1 += microseconds_now() - beg;

        beg = microseconds_now();
        find_balance = find_number(balance_tree, number, &count_cmp_balance);
        end2 += microseconds_now() - beg;

        if (i == 0)
        {
            if (find && find_balance)
                printf(GREEN "\nЭлемент со значением (%d) "
                             "был найден в бинарном дереве!\n" RESET,
                       number);
            else
                printf(VIOLET "\nЭлемент со значением (%d) "
                              "не был найден в бинарном дереве!\n" RESET,
                       number);
            printf(YELLOW "\nКоличество сравнений при поиске в "
                          "обычном бинарном дереве поиска = %d\n",
                   count_cmp);
            printf(YELLOW "\nКоличество сравнений при поиске в "
                          "сбалансированном бинарном дереве поиска = %d\n",
                   count_cmp_balance);
        }
    }

    printf(YELLOW "\nВремя поиска в обычном "
                  "бинарном дереве (мкс) = %Lf\n" RESET,
           end1 / N_REPS);
    printf(YELLOW "\nВремя поиска в сбалансированном "
                  "бинарном дереве (мкс) = %Lf\n" RESET,
           end2 / N_REPS);

    int size = tree_size(tree), size_balance = tree_size(balance_tree);

    printf(YELLOW "\nТреуемое количество памяти (байты) "
                  "для хранения обычного дерева = %d\n" RESET,
           size);
    printf(YELLOW "\nТреуемое количество памяти (байты) "
                  "для хранения сбалансированного дерева = %d\n" RESET,
           size_balance);

    return rc;
}

static int read_hash_numbers(char *file_name, hash_table_t *table)
{
    int rc = 0;

    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        puts(RED "\nОшибка открытия файла!\n" RESET);
        return ERR_OPEN_FILE;
    }

    int count, data;

    if (fscanf(file, "%d", &count) != 1)
    {
        fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
        return ERR_READ_DATA;
    }

    table->size = count;

    if ((rc = create_hash_table(table)) != 0)
    {
        puts(RED "\nОшибка выделения памяти!\n" RESET);
        return ERR_MEM_ALLOC;
    }
    default_hash_table(table);

    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &data) != 1)
        {
            fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
            return ERR_READ_DATA;
        }

        int hash_n = hash(data, count);

        data_t *node = create_data(data);

        if (!node)
        {
            puts(RED "\nОшибка выделения памяти!\n" RESET);
            fclose(file);
            return ERR_MEM_ALLOC;
        }

        table->data[hash_n].head = push_front(table->data[hash_n].head, node);

        if (!table->data[hash_n].is_full)
        {
            table->data[hash_n].is_full = true;
            table->data[hash_n].hash = hash_n;
        }
        else
            table->data[hash_n].collision++;
    }

    fclose(file);

    return rc;
}

static int make_hash_table(int *count_data, hash_table_t *table)
{
    char file_name[MAX_STR_SIZE];
    char data_file[MAX_STR_SIZE] = DATA_DIR;
    char data_gv[MAX_STR_SIZE];

    int rc = 0;

    if ((rc = read_file_name(file_name)) != 0)
        return rc;

    strcat(data_file, file_name);

    if (!is_exist_file(data_file))
    {
        if ((rc = read_count_data(count_data)) != 0)
            return rc;

        gen_data_file(data_file, *count_data);
    }

    strcpy(data_gv, data_file);
    strcat(data_gv, GV);

    if ((rc = read_hash_numbers(data_file, table)) != 0)
        return rc;

    if (rc == 0)
        puts(GREEN "\nДанные были успешно прочитаны!" RESET);

    return rc;
}

static int get_count_collisions(hash_table_t *table)
{
    int count_collisions = 0;

    for (int i = 0; i < table->size; i++)
        count_collisions += table->data[i].collision;

    return count_collisions;
}

static void print_table(hash_table_t *table)
{
    int count_collisions = get_count_collisions(table);

    printf(YELLOW "\nОбщее количество коллизий = %d\n" RESET,
           count_collisions);

    print_hash_table(table);
}

static void find_hash_number(hash_table_t *table, int data)
{
    int hash_n = hash(data, table->size);

    int count_compare = 0;

    data_t *tmp_head = table->data[hash_n].head;

    bool is_find = false;

    while (tmp_head)
    {
        count_compare++;
        if (tmp_head->data == data)
            printf(GREEN "\nЭлемент со значением (%d) "
                         "был найден в хеш-таблице!\n" RESET,
                   data),
                is_find = true;
        tmp_head = tmp_head->next;
    }

    if (!is_find)
        printf(GREEN "\nЭлемент со значением (%d) "
                     "не был найден в хеш-таблице!\n" RESET,
               data);

    printf(YELLOW "\nКоличество сравнений при поиске в "
                          "хеш-таблице = %d\n",
                   count_compare);
}

static int find_hash_data(hash_table_t *table)
{
    int rc = 0;

    int number;

    if ((rc = read_find_elem(&number)) != 0)
        return rc;

    find_hash_number(table, number);

    return rc;
}

int process(void)
{
    int rc = 0;

    tree_t tree, balance_tree;
    tree.root = NULL, balance_tree.root = NULL;

    hash_table_t table;
    table.data = NULL;

    int count_data;

    while (true)
    {
        print_menu();

        int menu_item;

        if ((rc = read_menu_item(&menu_item)) != 0)
            goto free;

        switch (menu_item)
        {
        case 0:
            free_table(&table);
            free_tree(&tree);
            free_tree(&balance_tree);
            exit(0);
            break;
        case 1:
            if ((rc = read_data(&count_data, &tree, &balance_tree)) != 0)
                goto free;
            break;
        case 2:
            if ((rc = add_data(&tree, &balance_tree)) != 0)
                goto free;
            break;
        case 3:
            rc = del_data(&tree, &balance_tree);
            if ((rc != 0 && rc != 2))
                goto free;
            break;
        case 4:
            if ((rc = trees_to_dot(&tree, &balance_tree)) != 0)
                goto free;
            break;
        case 5:
            if ((rc = find_data(&tree, &balance_tree)) != 0)
                goto free;
            break;
        case 6:
            if ((rc = make_hash_table(&count_data, &table)) != 0)
                goto free;
            break;
        case 8:
            print_table(&table);
            break;
        case 9:
            if ((rc = find_hash_data(&table)) != 0)
                goto free;
            break;
        default:
            break;
        }
    }

free:
    free_table(&table);
    free_tree(&tree);
    free_tree(&balance_tree);

    return rc;
}
