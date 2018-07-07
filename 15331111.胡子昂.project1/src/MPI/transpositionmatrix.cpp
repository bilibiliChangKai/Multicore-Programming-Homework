/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个mpi程序，用于帮矩阵转置。
Date: 2018年6月20日 星期三 下午7:51
****************************************************************************/

#include "../../inc/matrix.h"
#include "../../inc/timer.h"
#include <cmath>
#include <iostream>
#include <mpi.h>
using namespace std;

int *matrix;
int *matrixT;

int main(int argc, char **argv)
{
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
      matrix = new int[n * n];
      matrixT = new int[n * n];
    }
    ~DeferFunc() {
      delete[] matrix;
      delete[] matrixT;
    }
  } _;

  if (myid == 0) {
    initMatrix(matrix, n);
    begin = getTimeStamp();
  }
 
  MPI_Bcast(&n, 1, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Bcast(matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

  // STEP : 2
  // process begin calculate
  fprintf(stderr, "%d process enter calculate part!\n", myid);
  long beginindex = myid == 0 ? 0 : n * n / numprocs * myid;
  long endindex =
      myid == numprocs - 1 ? n * n : (n * n / numprocs) * (myid + 1);
  fprintf(stderr, "%d process calculate [(%ld, %ld), (%ld, %ld)!\n", myid,
          ROW(beginindex), COL(beginindex), ROW(endindex), COL(endindex));

  buffer = new int[n * n];
  for (int i = 0; i < n * n; i++) {
    buffer[i] = 0;
  }
  for (long pos = beginindex; pos < endindex; pos++)
  {
    buffer[POS(COL(pos), ROW(pos))] = matrix[pos];
  }

  // STEP : 3
  // process reduce
  MPI_Reduce(buffer, matrixT, n * n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  // STEP : 4
  // process end
  if (myid == 0) {
    end = getTimeStamp();
    fprintf(stdout, "Times : %ldus\n", getDuration(begin, end));
    printMatrix((int *)matrix, n);
    printMatrix((int *)matrixT, n);
  }
  MPI_Finalize();
}