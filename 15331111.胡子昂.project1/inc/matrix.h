/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: 存储一些矩阵使用的宏定义和函数。
Date: 2018年6月20日 星期三 下午7:53
****************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
using namespace std;

#define ROW(x) (x / n)
#define COL(x) (x % n)
#define POS(x, y) (x * n + y)
#define BOOST_PP_STRINGIZE(x) #x

long n;

void scanN() {
  fprintf(stdout, "Please input n :");
  scanf("%ld", &n);
}

void initMatrix(int *matrix, int n) {
  srand((unsigned)time(0));
  for (long i = 0; i < n * n; i++) {
    matrix[i] = rand() % n;
  }
}

void printMatrix(int *matrix, int n) {
  cout << BOOST_PP_STRINGIZE(matrix) << endl;
  for (long i = 0; i < n; i++) {
    for (long j = 0; j < n; j++) {
      cout << matrix[POS(i, j)] << ' ';
    }
    cout << endl;
  }
}

#endif // MATRIX_H