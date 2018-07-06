/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个mpi程序，用于帮矩阵转置。
Date: 2018年6月20日 星期三 下午7:51
****************************************************************************/

#include "../../inc/matrix.h"
#include <cmath>
#include <iostream>
#include <mpi.h>
using namespace std;

const long n = 1000;
int matrix[n][n];
int matrixT[n][n];

// inline Position getPosition(long pos)
// {
//     long begin = 1;
//     long end = n - 1;
//     long tempsum = sum;
//     while (begin != end - 1)
//     {
//         long half = (tempsum - pow((end - begin + 1), 2) / 4) / 2;
//         // 奇数的话，则需要减mid中的一半
//         if ((end - begin + 1) % 2 == 1)
//             half -= (begin + end) / 4;
//     }
// }

int main(int argc, char **argv)
{
  int myid, numprocs;
  int namelen;
  char processname[MPI_MAX_PROCESSOR_NAME];
  double begin, end;

  // init matrix
  initMatrix((int *)matrix, n);

  // STEP : 1
  // init multicore process
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Get_processor_name(processname, &namelen);
  fprintf(stderr, "Process %d of %d on %s\n", myid, numprocs, processname);
  begin = MPI_Wtime();

  // STEP : 2
  // process begin calculate
  fprintf(stderr, "%d process enter calculate part!\n", myid);
  long beginindex = myid == 0 ? 0 : n * n / numprocs * myid;
  long endindex =
      myid == numprocs - 1 ? n * n : (n * n / numprocs) * (myid + 1);
  fprintf(stderr, "%d process calculate [(%ld, %ld), (%ld, %ld)!\n", myid,
          ROW(beginindex), COL(beginindex), ROW(endindex), COL(endindex));

  for (long i = beginindex; i < endindex; i++)
  {
    matrixT[COL(i)][ROW(i)] = matrix[ROW(i)][COL(i)];
  }

  // STEP : 3
  // process end
  end = MPI_Wtime();
  fprintf(stderr, "%d process over in %lf times!\n", myid, end - begin);
  MPI_Finalize();

  printMatrix((int *)matrix, n);
  printMatrix((int *)matrixT, n);
}