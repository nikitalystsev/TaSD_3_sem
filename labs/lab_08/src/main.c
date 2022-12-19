#include "defines.h"
#include "process.h"

int main(void)
{
    int rc = 0;

    rc = process();

    return rc;
}

// #include <conio.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// struct graph_v // структура вершины графа
// {
//     int number; // номер
//     int degree; // степень
//     int *nears; // список смежности
// };

// graph_v *ob; // указатель на массив вершин
// int n;       // число вершин

// int *mark;        // массив марок
// int *result;      // результат
// int numb_res = 0; // и количество вершин в результате
// int input()
// { // подпрограмма ввода
//     int i;
//     char inpname[30];
//     FILE *f;
//     printf("Enter the name of input file:\n");
//     scanf("%s", inpname);
//     if ((f = fopen(inpname, "r")) == NULL)
//     {
//         printf("Cannot open input file!\n");
//         getch();
//         return 0;
//     }
//     fscanf(f, "%d", &n); // считываем кол-во вершин
//     if (n == 0)
//     {
//         printf("Graph is empty!\n"); // пустой граф
//         getch();
//         return 0;
//     }
//     if (n == 1)
//     {
//         printf("Atomic graph.\n"); // атомарный граф
//         getch();
//         return 0;
//     }

//     ob = (graph_v *)calloc(n, sizeof(graph_v)); // память под граф
//     mark = (int *)calloc(n, sizeof(int));       // память под массив марок

//     int j;
//     for (i = 0; i < n; i++)
//     {
//         fscanf(f, "%d", &ob[i].number);
//         fscanf(f, "%d", &ob[i].degree); // номер и степень
//         if (ob[i].degree != 0)
//         {
//             ob[i].nears = (int *)calloc(ob[i].degree, sizeof(int)); // создаем список смежности
//             for (j = 0; j < ob[i].degree; j++)
//                 fscanf(f, "%d", &ob[i].nears[j]);
//         };
//     }
//     return 1;
// }

// void clear()
// { // очистка марок
//     for (int i = 0; i < n; i++)
//         mark[i] = 0;
// }

// int goneall()
// { // поиск не пойденных вершин
//     int i;
//     for (i = 0; i < n; i++)
//         if (mark[i] == 0)
//             return 0; // нашли нарушителя - значит да
//     return 1;         // иначе все нормально
// }

// void goth(int numb)
// { // обход в глубину
//     if (mark[numb] == 0)
//     {
//         mark[numb] = 1;
//         for (int i = 0; i < ob[numb].degree; i++)
//             goth(ob[numb].nears[i]);
//     }
// }

// void output()
// { // вывод
//     if (numb_res == 0)
//         printf("There are no such vertexes.\n");
//     else
//     {
//         printf("Vertexes:\n");
//         for (int i = 0; i < numb_res; i++)
//             printf("%d ", result[i]);
//         printf("\nare satisfied with task.");
//     }
// }

// int find_vertex()
// { // находим вершину, с которой можно начинать обход
//     for (int i = 0; i < n; i++)
//         if (mark[ob[i].number] == 0)
//             return ob[i].number;
//     return -1;
// }

// int is_svyaz(int numbs)
// {                        // проверка на связность
//     clear();             // очищаем массив марок
//     mark[numbs] = 2;     // помечаем удаленную вершину
//     goth(find_vertex()); // обходим
//     if (goneall())
//         return 1; // если все прошли, то связен
//     else
//         return 0;
// }

// int numb_rebs(int numbs)
// {                          // количество ребер
//     int sum = 0, numb_reb; // накопим и посчитаем
//     for (int i = 0; i < n; i++)
//         sum += ob[i].degree;
//     numb_reb = (sum / 2) - ob[numbs].degree;
//     return numb_reb;
// }

// void in_result(int numb)
// { // добавление вершины в результат
//     result[numb_res] = numb;
//     numb_res++;
// }

// int main()
// {
//     if (input() == 0)
//         return 0;                           // ввод
//     result = (int *)calloc(n, sizeof(int)); // выделяем память
//     int i;
//     for (i = 0; i < n; i++)
//         // если граф связен и количество ребер равно n-1, то граф - дерево
//         if (is_svyaz(ob[i].number) && numb_rebs(ob[i].number) == n - 2)
//             in_result(ob[i].number);
//     output();
//     getch();
//     return 1;
// }
