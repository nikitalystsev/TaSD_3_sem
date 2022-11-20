#include "print.h"

void print_intermediate(const data_modeling_t *const data_modeling)
{
    printf(GREEN "\n\n|--------------------------------------------|------------|\n");
    printf("│    Количество вышедших заявок заявок       │ %10d │\n", data_modeling->total_output_application);
    printf("|--------------------------------------------|------------|\n");
    printf("│          Текушая длина очереди             │ %10d |\n", data_modeling->current_length_queue);
    printf("|--------------------------------------------|------------|\n");
    printf("│          Средняя длина очереди             │  %9.6lf │\n", data_modeling->average_length_queue);
    printf("|--------------------------------------------|------------|\n" RESET);
}

void print_result(const data_modeling_t *const data_modeling, const parametres_t *const parametres)
{
    double theoretic_total_simulation_time = ((parametres->max_time_add - parametres->min_time_add) / 2) * parametres->count_application;
    double theoretic_time_input_all_application = ((parametres->max_time_add - parametres->min_time_add) / 2) * parametres->count_application;
    double theoretic_time_output_all_application = ((parametres->max_time_processing - parametres->min_time_processing) / 2) * parametres->count_application * COUNT_PROCESS;
    double theoretic_downtime_machine = theoretic_time_input_all_application - theoretic_time_output_all_application;
    int16_t theoretic_count_calls_machine = parametres->count_application * COUNT_PROCESS;

    double fault_input = (100 * abs(data_modeling->total_input_application - parametres->count_application * COUNT_PROCESS)) / (parametres->count_application * COUNT_PROCESS);
    double fault_output = (100 * fabs(data_modeling->total_simulation_time - theoretic_total_simulation_time)) / theoretic_total_simulation_time;

    printf(YELLOW "\n|-------------------------------------|--------------------|--------------------|\n");
    printf("│             Название                │    Практические    │    Теоретические   │\n");
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│     Общее время моделирования       │ %13.6lf е.в. │ %10.0lf е.в.    │\n", data_modeling->total_simulation_time, theoretic_total_simulation_time);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│   Общее время добавления заявок     │ %13.6lf е.в. │ %10.0lf е.в.    │\n", data_modeling->time_input_all_application, theoretic_time_input_all_application);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│   Общее время обработки заявок      │ %13.6lf е.в. │ %10.0lf е.в.    │\n", data_modeling->time_output_all_application, theoretic_time_output_all_application);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│         Время простоя ОА            │ %13.6lf е.в. │ %10.0lf е.в.    │\n", data_modeling->downtime_machine, theoretic_downtime_machine);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│      Количество срабатываний        │ %10d е.в.    │ %5d +- 5 е.в.    │\n", data_modeling->count_calls_machine, theoretic_count_calls_machine);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│     Количество вошедших заявок      │             %10d                  │\n", data_modeling->total_input_application);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│     Количество вышедших заявок      │             %10d                  │\n", data_modeling->total_output_application);
    printf("|-------------------------------------|--------------------|--------------------|\n");
    printf("│                  Расчет погрешности                      │     Результат      │\n");
    printf("|----------------------------------------------------------|--------------------|\n");
    printf("│           -------Погрешность по входу--------            │                    │\n");
    printf("│  100 * |Кол-во пришед. - Кол-во теорет| / Кол-во теорет. │ %10.6lf процент │\n", fault_input);
    printf("|----------------------------------------------------------|--------------------|\n");
    printf("│           -------Погрешность по выходу--------           │                    │\n");
    printf("│  100 * |Практ. время модел-я - Теорит. | / Теорет.       │ %10.6lf процент │\n", fault_output);
    printf("|----------------------------------------------------------|--------------------|\n" RESET);
}
