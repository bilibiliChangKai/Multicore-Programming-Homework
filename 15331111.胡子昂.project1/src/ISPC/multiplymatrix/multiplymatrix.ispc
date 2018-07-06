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

export void multiplymatrix(uniform int matrixA[], uniform int matrixB[], uniform int matrixC[], uniform int n) {
        for (uniform int i = 0; i < n * n; i += programCount)
        {
            int idx = i + programIndex;
            matrixC[idx] = 0;
            for (uniform int j = 0; j < n; j++)
            {
                matrixC[idx] += matrixA[idx / n * n + j] * matrixB[j * n + idx % n];
            }
        }
}
