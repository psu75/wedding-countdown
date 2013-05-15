#include "date_def.h"

int diffdays(s_date d1, s_date d2)
{
    int factor1, factor2, result;
    factor1 = factor(d1);
    factor2 = factor(d2);
    result = factor2 - factor1;
    return (result);
}

int factor(s_date date)
{
    int result;
    result = 365 * date.year + date.day + 31 * (date.month - 1);
    
    if (date.month < 3)
        result = result + ((date.year -1)/4) - (.75 * ((date.year - 1)/100)+1);
    else
        result = result - (.4 * date.month + 2.3) + (date.year / 4) - (((.75 * (date.year / 100) + 1)));
    return (result);
}

