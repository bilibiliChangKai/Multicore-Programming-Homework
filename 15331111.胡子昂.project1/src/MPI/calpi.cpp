/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个mpi程序，用来计算pi。
  计算pi的方法 : pi/4 = 1 - 1/3 + 1/5 .....
Date: 2018年6月20日 星期三 上午9:33
****************************************************************************/

#include "../../inc/calpi.h"
#include "../../inc/timer.h"
#include <mpi.h>
#include <stdio.h>
// using namespace std;

int main(int argc, char **argv) {
  int myid, numprocs;
  long begin, end;
  double sum = 0;
  double mypart, x;  

  // STEP : 1
  // init multicore process
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  // fprintf(stderr, "Process %d of %d create!\n", myid, numprocs);
  if (myid == 0) {
    scanIteratime();
    begin = getTimeStamp();
  }
  MPI_Bcast(&iteratime, 1, MPI_LONG, 0, MPI_COMM_WORLD);
  step = 1 / (double)iteratime;

  // STEP : 2
  // process begin calculate
  // fprintf(stderr, "%d process enter calculate part!\n", myid);

  long beginindex = myid == 0 ? 1 : iteratime / numprocs * myid;
  long endindex = myid == numprocs - 1 ? iteratime: (iteratime / numprocs) * (myid + 1);
  // fprintf(stderr, "%d process calculate [%ld, %ld)!\n", myid, beginindex,
  //         endindex);

  mypart = 0;
  for (long i = beginindex; i < endindex; i++) {
    x = (i + 0.5) * step;
    mypart += 4.0 / (1.0 + x * x);
  }
  mypart *= step;

  // STEP : 3
  // process reduce
  MPI_Reduce(&mypart, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  // STEP : 4
  // process end
  if (myid == 0) {
    end = getTimeStamp();
    fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));
    fprintf(stdout, "pi is %lf!\n", sum);
  }
  MPI_Finalize();
  
  return 0;
}