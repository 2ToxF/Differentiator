#include "settings.h" // !!!

#include <math.h>
#include <sys/stat.h>

#include "utils.h"

const double PRECISION_COEF = 1E-7;


CodeError Fsize(const char* file_name, int* input_buffer_length)
{
    struct stat input_file_stat = {};
    if (stat(file_name, &input_file_stat) != 0)
        return FILLING_FILE_STAT_ERR;

    *input_buffer_length = input_file_stat.st_size + 1;
    return NO_ERR;
}


bool IsEqual(double x1, double x2)
{
    return fabs(x1-x2) <= PRECISION_COEF;
}

bool IsZero(double x)
{
    return IsEqual(x, 0);
}
