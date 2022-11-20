#include "gen_number.h"

double random_double(double min, double max)
{
    double x = rand() / (double)RAND_MAX; /* [0, 1.0] */

    return min + x * (max - min);
}
