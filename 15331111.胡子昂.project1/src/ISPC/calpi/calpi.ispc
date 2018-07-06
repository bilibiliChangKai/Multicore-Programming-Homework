/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: ispc函数
Date: 2018年6月22日 星期五 下午12:16
****************************************************************************/

export uniform int getNumprocs() {
  return programCount;
}

export uniform double calpi(uniform int iteratime, uniform double step) {
    uniform double sum = 0; 
    int myid = programIndex;
    double mypart = 0;
    for (uniform int i = 0; i < iteratime; i += programCount) {
      double x = (i + myid + 0.5) * step;
      mypart += 4.0 / (1.0 + x * x);
    }
    mypart *= step;
    sum = reduce_add(mypart);
    return sum;
}
