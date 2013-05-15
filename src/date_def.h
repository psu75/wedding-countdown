#pragma once

#ifndef __DATE_DEF__
#define __DATE_DEF__

typedef struct {
    int month;
    int day;
    int year;
} s_date;

extern char * HOLIDAYS[];

int diffdays(s_date d1, s_date d2);

int factor(s_date date);

#endif