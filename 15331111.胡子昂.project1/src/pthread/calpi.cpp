/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 这是一个pthread程序，用来计算pi。
  计算pi的方法 : pi = f (4 / (1 - x * x))
Date: 2018年6月20日 星期三 上午9:33
****************************************************************************/

#include "../../inc/calpi.h"
#include "../../inc/timer.h"
#include <iostream>
#include <pthread.h>
using namespace std;

int numprocs;
pthread_t *pids;
double *pipart;

// 线程运行函数
void *threadFunc(void *arg) {
  // STEP : 2
  // process begin calculate
  int myid = (int)(long)arg;
  // fprintf(stderr, "%d process enter calculate part!\n", myid);
  long beginindex = myid == 0 ? 1 : iteratime / numprocs * myid;
  long endindex =
      myid == numprocs - 1 ? iteratime : (iteratime / numprocs) * (myid + 1);
  // fprintf(stderr, "%d process calculate [%ld, %ld)!\n", myid, beginindex,
  //         endindex);

  // 计算pi
  pipart[myid] = 0;
  for (long i = beginindex; i < endindex; i++) {
    double x = (i + 0.5) * step;
    pipart[myid] += 4.0 / (1.0 + x * x);
  }
  pipart[myid] *= step;

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  scanIteratime();
  time_t begin, end;
  double sum = 0;
  if (argc != 2) {
    numprocs = 4;
  } else {
    numprocs = atoi(argv[1]);
  }

  // init pids, pipart
  struct DeferFunc {
    DeferFunc() {
      pids = new pthread_t[numprocs];
      pipart = new double[numprocs];
    }
    ~DeferFunc() {
      delete[] pids;
      delete[] pipart;
    }
  } _;
  begin = getTimeStamp();

  // STEP : 1
  // init multicore process
  for (int i = 0; i < numprocs; i++) {
    // fprintf(stderr, "Process %d of %d create!\n", i, numprocs);
    pthread_create(&pids[i], NULL, threadFunc, (void *)i);
  }

  // STEP : 3
  // process reduce
  for (int i = 0; i < numprocs; i++) {
    pthread_join(pids[i], NULL);
    sum += pipart[i];
  }

  // STEP : 4
  // process end
  end = getTimeStamp();
  fprintf(stderr, "Times : %ldus\n", getDuration(begin, end));

  fprintf(stdout, "pi is %lf!\n", sum);
  return 0;
}