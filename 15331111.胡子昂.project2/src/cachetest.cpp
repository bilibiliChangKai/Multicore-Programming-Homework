/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 用来测试cache的程序，通过递增内存大小进行测速
Date: 2018年7月6日 星期五 下午4:52
****************************************************************************/

#include <chrono>
#include <iostream>
using namespace std;
using namespace chrono;

long long n;
long long l;
char *arr;

int main(int argv, char **argc)
{
  if (argv != 3)
  {
    fprintf(stderr, "please input n and l!\n");
    return 1;
  }

  // long begin, end;
  char temp;
  n = atoi(argc[1]);
  l = atoi(argc[2]);
  // cout << sizeof(temp) << endl;

  arr = new char[n * l];

  auto begin = high_resolution_clock::now();
  // 读取全部数据一遍
  for (long long i = 0; i < n * l; i++)
  {
    temp = arr[i];
  }
  // 只读取1024个等距数据
  // for (long long i = 0; i < 1024; i++) {
  //   temp = arr[n * l / 1024 * i];
  // }
  // 读取n个数据，间隔为l
  // for (long long i = 0; i < n; i++)
  // {
  //   temp = arr[i * l];
  // }
  auto end = high_resolution_clock::now();

  auto d = duration_cast<nanoseconds>(end - begin);
  fprintf(stdout, "%lld,%lld,%lld\n", n, d.count(), d.count() / n);
}