/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个ispc程序，用来计算pi。
  计算pi的方法 : pi/4 = 1 - 1/3 + 1/5 .....
Date: 2018年6月22日 星期五 上午10:03
****************************************************************************/

#include "../../../inc/matrix.h"
#include "../../../inc/timer.h"
#include <iostream>
using namespace std;

// Include the header file that the ispc compiler generates
#include "transpositionmatrix_ispc.h"
using namespace ispc;

int *matrix;
int *matrixT;

int main()
{
  scanN();
  long begin, end;

  // init pids, matrixs
  struct DeferFunc
  {
    DeferFunc()
    {
      matrix = new int[n * n];
      matrixT = new int[n * n];
    }
    ~DeferFunc()
    {
      delete[] matrix;
      delete[] matrixT;
    }
  } _;
  initMatrix(matrix, n);
  begin = getTimeStamp();

  // STEP : 1
  // process begin calculate
  transpositionmatrix((int *)matrix, (int *)matrixT, n);

  // STEP : 2
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  // printMatrix(matrix, n);
  // printMatrix(matrixT, n);
}
