#include "process.h"

static int read_menu_item(int *item)
{
    if (scanf("%d", item) != 1)
    {
        puts(RED "\nНекорректный ввод пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    if (*item < 0 || *item > 10)
    {
        puts(RED "\nНеверный номер пункта меню!" RESET);
        return ERR_MENU_ITEM;
    }

    return EXIT_SUCCESS;
}
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

static int check_node(adj_list_t *node)
{
    if (node)
        return 0;

    return 1;
}

static int read_adj_list(FILE *file, vertex_t *vertex, int count_node)
{
    int rc = 0;

    int number;

    if (fscanf(file, "%d", &number) != 1)
    {
        fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
        return ERR_READ_DATA;
    }

    // printf("number = %d\n", number);

    vertex->number = number;

    for (int i = 0; i < count_node; i++)
    {
        if (fscanf(file, "%d", &number) != 1)
        {
            fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
            return ERR_READ_DATA;
        }

        // printf("number in = %d\n", number);

        adj_list_t *node = create_node(number);

        if ((rc = check_node(node)) != 0)
        {
            puts(RED "\nОшибка выделения памяти!\n" RESET);
            return rc;
        }

        vertex->head = push_end(vertex->head, node);
    }

    return rc;
}

static int read_graph(graph_t *graph)
{
    int rc = 0;

    char file_name[MAX_STR_SIZE];
    char data_file[MAX_STR_SIZE] = DATA_DIR;

    if ((rc = read_file_name(file_name)) != 0)
        return rc;

    strcat(data_file, file_name);

    FILE *file = fopen(data_file, "r");

    if (!file)
    {
        puts(RED "Ошибка открытия файла!\n" RESET);
        return ERR_OPEN_FILE;
    }

    if (fscanf(file, "%d", &graph->size) != 1)
    {
        fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
        return ERR_READ_DATA;
    }

    if ((rc = create_graph(graph)) != 0)
    {
        fclose(file);
        return rc;
    }

    default_graph(graph);

    for (int i = 0; i < graph->size; i++)
    {
        // printf("чтение продолжается\n");

        int count_node;

        if (fscanf(file, "%d", &count_node) != 1)
        {
            fclose(file), puts(RED "\nОшибка чтения данных из файла!" RESET);
            return ERR_READ_DATA;
        }

        // printf("count_node = %d\n", count_node);

        if ((rc = read_adj_list(file, &graph->data[i], count_node)) != 0)
        {
            fclose(file);
            return rc;
        }
    }

    fclose(file);

    // printf("конец чтения, rc = %d\n", rc);

    if (rc == 0)
        puts(GREEN "\nДанные были успешно прочитаны!" RESET);

    return rc;
}

int process(void)
{
    int rc = 0;

    graph_t graph;
    graph.size = 0, graph.data = NULL;

    while (true)
    {
        print_menu();

        int menu_item;

        if ((rc = read_menu_item(&menu_item)) != 0)
            goto free;

        switch (menu_item)
        {
        case 0:
            free_graph(&graph);
            exit(0);
            break;
        case 1:
            if ((rc = read_graph(&graph)) != 0)
                goto free;
            break;
        default:
            break;
        }
    }

free:
    free_graph(&graph);

    return rc;
}
