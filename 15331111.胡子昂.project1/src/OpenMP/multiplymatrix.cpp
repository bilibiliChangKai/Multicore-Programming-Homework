/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个pthread程序，用于进行矩阵乘法。
Date: 2018年6月20日 星期三 下午7:51
****************************************************************************/

#include "../../inc/matrix.h"
#include "../../inc/timer.h"
#include <iostream>
#include <omp.h>
using namespace std;

int *matrixA;
int *matrixB;
int *matrixC;

int main(int argc, char **argv)
{
#ifdef OPENMP
  printf("Compiled by an OpenMP-compliant implementation.\n");
#endif
  scanN();
  long begin, end;

  // init matrixs
  struct DeferFunc
  {
    DeferFunc()
    {
      matrixA = new int[n * n];
      matrixB = new int[n * n];
      matrixC = new int[n * n];
    }
    ~DeferFunc()
    {
      delete[] matrixA;
      delete[] matrixB;
      delete[] matrixC;
    }
  } _;
  initMatrix(matrixA, n);
  initMatrix(matrixB, n);
  begin = getTimeStamp();

// STEP : 1
// init multicore process
#pragma omp parallel for shared(matrixA, matrixB, matrixC)
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
      matrixC[pos] = 0;
      for (int i = 0; i < n; i++)
      {
        matrixC[pos] += matrixA[POS(ROW(pos), i)] * matrixB[POS(i, COL(pos))];
      }
    }
  }

  // STEP : 3
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  // printMatrix(matrixA, n);
  // printMatrix(matrixB, n);
  // printMatrix(matrixC, n);
}