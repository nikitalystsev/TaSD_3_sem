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

static int read_data(const char *file_name, tree_t *tree, bool is_balance)
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

static int read_data_from_file(int *count_data, tree_t *tree,
                               tree_t *balance_tree)
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

    if ((rc = read_data(data_file, tree, false)) != 0)
        return rc;

    if ((rc = read_data(data_file, balance_tree, true)) != 0)
        return rc;

    if (rc == 0)
        puts(GREEN "\nДанные были успешно прочитаны!" RESET);

    return rc;
}

int process(void)
{
    int rc = 0;

    tree_t tree, balance_tree;
    tree.root = NULL, balance_tree.root = NULL;

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
            free_tree(&tree);
            free_tree(&balance_tree);
            exit(0);
            break;
        case 1:
            if ((rc = read_data_from_file(&count_data, &tree,
                                          &balance_tree)) != 0)
                goto free;
            break;
        default:
            break;
        }
    }

free:
    free_tree(&tree);
    free_tree(&balance_tree);

    return rc;
}
