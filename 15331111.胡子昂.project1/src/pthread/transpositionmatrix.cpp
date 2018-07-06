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
#include <pthread.h>
using namespace std;

int *matrix;
int *matrixT;

int numprocs;
pthread_t *pids;

// 线程运行函数
void *threadFunc(void *arg)
{
  // STEP : 2
  // process begin calculate
  int myid = (int)(long)arg;
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
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  scanN();
  long begin, end;
  if (argc != 2)
  {
    numprocs = 4;
  }
  else
  {
    numprocs = atoi(argv[1]);
  }

  // init pids, matrixs
  struct DeferFunc
  {
    DeferFunc()
    {
      pids = new pthread_t[numprocs];
      matrix = new int[n * n];
      matrixT = new int[n * n];
    }
    ~DeferFunc()
    {
      delete[] pids;
      delete[] matrix;
      delete[] matrixT;
    }
  } _;
  initMatrix(matrix, n);
  begin = getTimeStamp();

  // STEP : 1
  // init multicore process
  for (int i = 0; i < numprocs; i++)
  {
    // fprintf(stderr, "Process %d of %d create!\n", i, numprocs);
    pthread_create(&pids[i], NULL, threadFunc, (void *)i);
  }

  // STEP : 3
  // process end
  for (int i = 0; i < numprocs; i++)
  {
    pthread_join(pids[i], NULL);
  }
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  // printMatrix(matrix, n);
  // printMatrix(matrixT, n);
}