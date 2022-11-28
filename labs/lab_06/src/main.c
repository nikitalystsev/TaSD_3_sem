#include "defines.h"
#include "tree_visual.h"
#include "gen_data.h"
#include "process.h"

int main(void)
{
    int rc = 0;

    rc = process();

//     char *filename = PATH DATA;

//     gen_data_file(filename);

//     FILE *file = fopen(filename, "r");

//     if (!file)
//         return 2;

//     tree_t tree;
//     tree.root = NULL;

//     int count;
//     rc = fscanf(file, "%d", &count);

//     for (int i = 0; i < count; i++)
//     {
//         int data;

//         rc = fscanf(file, "%d", &data);

//         vertex_t *vertex = create_vertex(data);
//         if (!vertex)
//             goto free;

//         tree.root = add_vertex(tree.root, vertex);
//     }

//     fclose(file);

//     filename = PATH DATA GV;

//     rc = export_to_dot(filename, "my_tree", &tree);
//     printf("rc = %d\n", rc);
//     delete_vertex(&tree.root, 58);

//     filename = PATH CHANGE DATA GV;
    
//     rc = export_to_dot(filename, "my_tree", &tree);

// free:
//     free_tree(&tree);

    return rc;
}
