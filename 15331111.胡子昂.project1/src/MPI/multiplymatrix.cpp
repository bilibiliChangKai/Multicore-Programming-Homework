/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个mpi程序，用于进行矩阵乘法。
Date: 2018年6月20日 星期三 下午7:51
****************************************************************************/

#include "../../inc/matrix.h"
#include "../../inc/timer.h"
#include <iostream>
#include <mpi.h>
using namespace std;

int *matrixA;
int *matrixB;
int *matrixC;

int main(int argc, char **argv) {
  int myid, numprocs;
  int *buffer;
  long begin, end;

  // STEP : 1
  // init multicore process
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  if (myid == 0) {
    scanN();
  }

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

  if (myid == 0) {
    initMatrix(matrixA, n);
    initMatrix(matrixB, n);
    begin = getTimeStamp();
  }
  
  MPI_Bcast(&n, 1, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrixA, n * n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrixB, n * n, MPI_INT, 0, MPI_COMM_WORLD);

  // STEP : 2
  // process begin calculate
  // fprintf(stderr, "%d process enter calculate part!\n", myid);
  long beginindex = myid == 0 ? 0 : n * n / numprocs * myid;
  long endindex =
      myid == numprocs - 1 ? n * n : (n * n / numprocs) * (myid + 1);
  // fprintf(stderr, "%d process calculate [(%ld, %ld), (%ld, %ld)!\n", myid,
  //         ROW(beginindex), COL(beginindex), ROW(endindex), COL(endindex));

  buffer = new int[n * n];
  for (int i = 0; i < n * n; i++) {
    buffer[i] = 0;
  }
  for (long pos = beginindex; pos < endindex; pos++) {
    for (int i = 0; i < n; i++) {
      buffer[pos] += matrixA[POS(ROW(pos), i)] * matrixB[POS(i, COL(pos))];
    }
  }

  // STEP : 3
  // process reduce
  MPI_Reduce(buffer, matrixC, n * n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  // STEP : 4
  // process end
  if (myid == 0) {
    end = getTimeStamp();
    fprintf(stdout, "Times : %ldus\n", getDuration(begin, end));
    printMatrix(matrixA, n);
    printMatrix(matrixB, n);
    printMatrix(matrixC, n);
  }
  MPI_Finalize();
}