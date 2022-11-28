#include "process.h"

static int read_menu_item(int *item)
{
    if (scanf("%d", item) != 1)
    {
        puts(RED "\nНекорректный ввод пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    if (*item < 0 || *item > 7)
    {
        puts(RED "\nНеверный номер пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}

int read_data(const char *file_name, tree_t *tree)
{
    int rc = 0;

    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        puts(RED "Ошибка открытия файла!\n" RESET);
        return ERR_OPEN_FILE;
    }

    int count, data;

    if (fscanf(file, "%d", &count) != 1)
    {
        fclose(file), puts(RED "Ошибка чтения данных из файла!\n" RESET);
        return ERR_READ_DATA;
    }

    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &data) != 1)
        {
            fclose(file), puts(RED "Ошибка чтения данных из файла!\n" RESET);
            return ERR_READ_DATA;
        }

        vertex_t *vertex = create_vertex(data);
        if (!vertex)
        {
            fclose(file);
            return ERR_READ_DATA;
        }

        tree->root = add_vertex(tree->root, vertex);
    }

    fclose(file);

    if (rc == 0)
        puts(GREEN "\nДанные были успешно прочитаны!" RESET);

    return rc;
}

int read_elem_tree(int *num)
{
    puts(TURQ "\nВведите элемент дерева:" RESET);

    if (scanf("%d", num) != 1)
    {
        puts(RED "\nНекорректный ввод элемента дерева!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}

int add(tree_t *tree, int data)
{
    int rc = 0;

    vertex_t *vertex = create_vertex(data);

    if (!vertex)
    {
        puts(RED "Ошибка добавления данных!\n" RESET);
        return ERR_READ_DATA;
    }

    tree->root = add_vertex(tree->root, vertex);

    if (rc == 0)
        puts(GREEN "\nЭлемент был успешно добавлен в дерево" RESET);

    return rc;
}

int add_file(const char *file_name, int data)
{
    int rc = 0;

    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        return ERR_OPEN_FILE;
    }

    int count;

    if (fscanf(file, "%d", &count) != 1)
    {
        fclose(file);
        return ERR_READ_DATA;
    }
    // считывание данных из файла в массив
    int *data_arr = malloc((count + 1) * sizeof(int));
    if (!data_arr)
    {
        fclose(file);
        return ERR_ALLOC_MEM;
    }

    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &data_arr[i]) != 1)
        {
            fclose(file);
            return ERR_READ_DATA;
        }
    }
    // добавление нового элемента
    data_arr[count] = data;

    fclose(file);
    // запись измененного массива в файл
    file = fopen(file_name, "w");
    if (!file)
    {
        return ERR_OPEN_FILE;
    }

    fprintf(file, "%d\n", count + 1);

    for (int i = 0; i <= count; i++)
    {
        fprintf(file, "%d\n", data_arr[i]);
    }

    fclose(file);

    free(data_arr);

    return rc;
}

void del(tree_t *tree, int data)
{
    if (delete_vertex(&tree->root, data) != 0)
    {
        puts(VIOLET "\nУдаляемого элемента в дереве нет" RESET);
    }
    else
    {
        printf(GREEN "\nЭлемент со значением (%d) "
                     "был успешно удален!\n" RESET,
               data);
    }
}

int process(void)
{
    int rc = 0;

    tree_t tree;
    tree.root = NULL;

    int elem;
    char *data = PATH DATA;
    char *data_gv = PATH DATA GV;

    int l = 0, t = 0;

    // gen_data_file(data);

    while (true)
    {
        print_menu();

        int menu_item;

        if ((rc = read_menu_item(&menu_item)) != 0)
            goto free;

        switch (menu_item)
        {
        case 0:
            free_tree(&tree);
            exit(0);
            break;
        case 1:
            if ((rc = read_data(data, &tree)) != 0)
                goto free;
            break;
        case 2:
            if ((rc = read_elem_tree(&elem)) != 0)
                goto free;
            if ((rc = add(&tree, elem)) != 0)
                goto free;
            break;
        case 3:
            if ((rc = read_elem_tree(&elem)) != 0)
                goto free;
            if ((rc = add_file(data, elem)) != 0)
                goto free;
            break;
        case 4:
            if ((rc = read_elem_tree(&elem)) != 0)
                goto free;
            del(&tree, elem);
            break;
        case 5:
            if (!is_empty_tree(&tree))
                print_tree(tree.root, 3);
            break;
        case 6:
            if (!is_empty_tree(&tree))
                if ((rc = export_to_dot(data_gv, "my_tree", &tree)) != 0)
                    goto free;
            break;
        case 7:
            if (!is_empty_tree(&tree))
                while ((l = get_count_vertex_in_level(tree.root, t++, 0)) != 0)
                {
                    printf(GREEN "level(%d) = %d\n" RESET, t - 1, l);
                }
            t = 0, l = 0;
            break;
        case 8:
            if (!is_empty_tree(&tree))
                free_tree(&tree);
        default:
            break;
        }
    }

free:
    free_tree(&tree);

    return rc;
}