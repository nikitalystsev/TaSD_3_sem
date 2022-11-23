#include "timing.h"

// Получение времени в микросекундах
long double microseconds_now(void)
{
    struct timeval val;

    if (gettimeofday(&val, NULL))
        return (unsigned long long) -1;

    return val.tv_sec * 1000000ULL + val.tv_usec;
}

// int time_del_elem_in_queue(void)
// {
//     queue_array_t queue_array;
//     queue_array.size = 
// }