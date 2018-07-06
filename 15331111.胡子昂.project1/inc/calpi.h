/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 用于设置n和p
Date: 2018年7月4日 星期三 上午8:04
****************************************************************************/

#ifndef CALPI_H
#define CALPI_H

#include <iostream>

long iteratime;
double step;

void scanIteratime()
{
    fprintf(stdout, "Please input n :");
    scanf("%ld", &iteratime);
    step = 1 / (double)iteratime;
}

#endif // CALPI_H