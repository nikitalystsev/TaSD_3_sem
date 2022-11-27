#include "tree.h"

vertex_t *create_vertex(int data)
{
    vertex_t *vertex = malloc(sizeof(vertex_t));

    if (vertex)
    {
        vertex->data = data;
        vertex->left = NULL;
        vertex->right = NULL;
    }

    return vertex;
}

vertex_t *add_vertex(vertex_t *root, vertex_t *vertex)
{
    if (!root)
    {
        return vertex;
    }

    if (vertex->data < root->data)
    {
        root->left = add_vertex(root->left, vertex);
    }
    else if (vertex->data > root->data)
    {
        root->right = add_vertex(root->right, vertex);
    }

    return root;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int get_tree_depth(vertex_t *root)
{
    if (!root)
    {
        return 0;
    }

    int ldepth = get_tree_depth(root->left);
    int rdepth = get_tree_depth(root->right);

    if (ldepth > rdepth)
    {
        return ldepth + 1;
    }

    return rdepth + 1;
}

int get_count_vertex_in_level(vertex_t *root, int n, int c)
{

    if (n == c)
    {
        return 1;
    }

    int count = 0;

    if (root->left)
    {
        count += get_count_vertex_in_level(root->left, n, c + 1);
    }
    if (root->right)
    {
        count += get_count_vertex_in_level(root->right, n, c + 1);
    }

    return count;
}
