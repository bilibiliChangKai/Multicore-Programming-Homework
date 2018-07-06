/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: ispc函数
Date: 2018年6月22日 星期五 下午12:16
****************************************************************************/

#define ROW(x) (x / n)
#define COL(x) (x % n)
#define POS(x, y) (x * n + y)

export uniform int getNumprocs() {
  return programCount;
}

export void transpositionmatrix(uniform int matrix[], uniform int matrixT[], uniform int n) {
        for (uniform int i = 0; i < n * n; i += programCount)
        {
            int idx = i + programIndex;
            matrixT[idx % n * n + idx / n] = matrix[idx];
        }
}
