/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个pthread程序，用于帮矩阵转置。
Date: 2018年6月20日 星期三 下午7:51
****************************************************************************/

#include "../../inc/matrix.h"
#include "../../inc/timer.h"
#include <cmath>
#include <iostream>
#include <omp.h>
using namespace std;

int *matrix;
int *matrixT;

int main(int argc, char **argv)
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
  // init multicore process
#pragma omp parallel for shared(matrix, matrixT)
  for (long t = 0; t < omp_get_num_threads(); t++)
  {
    // STEP : 2
    // process begin calculate
    int numprocs = omp_get_num_threads();
    int myid = omp_get_thread_num();
    // fprintf(stderr, "%d process enter calculate part!\n", myid);
    long beginindex = myid == 0 ? 0 : n * n / numprocs * myid;
    long endindex =
        myid == numprocs - 1 ? n * n : (n * n / numprocs) * (myid + 1);
    // fprintf(stderr, "%d process calculate [(%ld, %ld), (%ld, %ld)!\n", myid,
    //         ROW(beginindex), COL(beginindex), ROW(endindex), COL(endindex));

    for (long pos = beginindex; pos < endindex; pos++)
    {
      matrixT[POS(COL(pos), ROW(pos))] = matrix[pos];
    }
  }

  // STEP : 3
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  // printMatrix(matrix, n);
  // printMatrix(matrixT, n);
}