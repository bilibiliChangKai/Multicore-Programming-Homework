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
#include "multiplymatrix_ispc.h"
using namespace ispc;

int *matrixA;
int *matrixB;
int *matrixC;

int main() {
  scanN();
  long begin, end;

  // init matrixs
  struct DeferFunc {
    DeferFunc() {
      matrixA = new int[n * n];
      matrixB = new int[n * n];
      matrixC = new int[n * n];
    }
    ~DeferFunc() {
      delete[] matrixA;
      delete[] matrixB;
      delete[] matrixC;
    }
  } _;
  initMatrix(matrixA, n);
  initMatrix(matrixB, n);
  begin = getTimeStamp();

  // STEP : 1
  // process begin calculate
  multiplymatrix((int *)matrixA, (int *)matrixB, (int *)matrixC, n);

  // STEP : 2
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  // printMatrix((int *)matrixA, n);
  // printMatrix((int *)matrixB, n);
  // printMatrix((int *)matrixC, n);
  return 0;
}
