#include "settings.h" // !!!

#include <math.h>

#include "utils.h"

const double PRECISION_COEF = 1E-7;


bool IsEqual(double x1, double x2)
{
    return fabs(x1-x2) <= PRECISION_COEF;
}

bool IsZero(double x)
{
    return IsEqual(x, 0);
}
