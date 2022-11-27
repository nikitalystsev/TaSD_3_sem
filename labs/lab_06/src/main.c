#include "defines.h"
#include "tree_visual.h"
#include "gen_data.h"

int main(int argc, char const *argv[])
{
    int rc = 0;

    if (argc != 2)
        return 1;

    // gen_data_file(argv[1]);
    FILE *file = fopen(argv[1], "r");

    if (!file)
        return 2;

    tree_t tree;
    tree.root = NULL;

    int count;
    rc = fscanf(file, "%d", &count);

    for (int i = 0; i < count; i++)
    {
        int data;

        rc = fscanf(file, "%d", &data);

        printf("[+] data = %d\n", data);

        vertex_t *vertex = create_vertex(data);
        if (!vertex)
            return 3;

        tree.root = add_vertex(tree.root, vertex);
    }

    fclose(file);

    rc = export_to_dot("test.gv", "my_tree", &tree);

    int depth = get_tree_depth(tree.root);

    printf("depth = %d\n", depth);

    for (int i = 0; i < depth; i++)
    {
        int c = get_count_vertex_in_level(tree.root, i, 0);
        printf("lvl(%d) = %d\n", i, c);
    }


    return rc;
}
