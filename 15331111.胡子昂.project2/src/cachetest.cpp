/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 用来测试cache的程序，通过递增内存大小进行测速
Date: 2018年7月6日 星期五 下午4:52
****************************************************************************/

#include "timer.h"
#include <iostream>
using namespace std;

long long n;
int *arr;

int main(int argv, char **argc)
{
  if (argv != 2)
  {
    fprintf(stderr, "please input n!\n");
    return 1;
  }

  long begin, end;
  long temp;
  n = atoi(argc[1]);
  arr = new int[n];

  begin = getTimeStamp();
  for (long long i = 0; i < n; i++)
  {
    temp = arr[i];
  }
  end = getTimeStamp();

  fprintf(stdout, "%lld\t%ldus\n", n, getDuration(begin, end));
}