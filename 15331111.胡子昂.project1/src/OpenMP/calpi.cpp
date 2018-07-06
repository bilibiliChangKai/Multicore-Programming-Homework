/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个pthread程序，用来计算pi。
  计算pi的方法 : pi = f (4 / (1 - x * x))
Date: 2018年6月20日 星期三 上午9:33
****************************************************************************/

#include "../../inc/calpi.h"
#include "../../inc/timer.h"
#include <iostream>
#include <omp.h>
using namespace std;

int main(int argc, char **argv)
{
#ifdef OPENMP
    printf("Compiled by an OpenMP-compliant implementation.\n");
#endif
    scanIteratime();
    long begin, end;
    double sum = 0;

    begin = getTimeStamp();

// STEP : 1
// init multicore process
#pragma omp parallel for reduction(+ \
                                   : sum)
    for (long t = 0; t < omp_get_num_threads(); t++)
    {
        // STEP : 2
        // process begin calculate
        int numprocs = omp_get_num_threads();
        int myid = omp_get_thread_num();
        // fprintf(stderr, "%d process enter calculate part!\n", myid);
        long beginindex = myid == 0 ? 1 : iteratime / numprocs * myid;
        long endindex =
            myid == numprocs - 1 ? iteratime : (iteratime / numprocs) * (myid + 1);
        // fprintf(stderr, "%d process calculate [%ld, %ld)!\n", myid, beginindex,
        //         endindex);

        double mypart = 0;
        for (long i = beginindex; i < endindex; i++)
        {
            double x = (i + 0.5) * step;
            mypart += 4.0 / (1.0 + x * x);
        }
        mypart *= step;

        // STEP : 3
        // process reduce
        sum += mypart;
    }

    // STEP : 4
    // process end
    end = getTimeStamp();
    fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

    fprintf(stdout, "pi is %lf!\n", sum);
    return 0;
}