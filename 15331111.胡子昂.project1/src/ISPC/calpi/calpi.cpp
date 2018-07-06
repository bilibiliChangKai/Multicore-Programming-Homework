/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个ispc程序，用来计算pi。
  计算pi的方法 : pi = f (4 / (1 - x * x))
Date: 2018年6月22日 星期五 上午10:03
****************************************************************************/

#include "../../../inc/calpi.h"
#include "../../../inc/timer.h"
#include <iostream>
using namespace std;

// Include the header file that the ispc compiler generates
#include "calpi_ispc.h"
using namespace ispc;

int numprocs;
double *pipart;

int main() {
  scanIteratime();
  long begin, end;
  double sum = 0;

  // init pipart
  numprocs = getNumprocs();
  struct DeferFunc {
    DeferFunc() { pipart = new double[numprocs]; }
    ~DeferFunc() { delete[] pipart; }
  } _;
  begin = getTimeStamp();

  // STEP : 1
  // process begin calculate
  // STEP : 2
  // process reduce
  sum = calpi(iteratime, step);

  // STEP : 3
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  fprintf(stderr, "pi is %lf!\n", sum);
  return 0;
}
