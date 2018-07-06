/***************************************************************************
Copyright(C) 2018
Author: huziang
Description: Ldouble数据结构的头文件，用于存储高精度小数。
Date: 2018年6月20日 星期三 下午7:52
****************************************************************************/

#ifndef LDOUBLE_H
#define LDOUBLE_H

#include <iostream>
using namespace std;

template <int len>
class Ldouble
{
public:
  Ldouble()
  {
    pointpos = 1;
    for (int i = 0; i < len + 2; i++)
    {
      data[i] = '0';
    }
    data[len + 1] = '\0';
  }
  void setVal(const long a);
  void plus(const Ldouble &);
  void minus(const Ldouble &);
  void multi(const long m);
  void divi(const long b);
  void print();

private:
  char data[len + 2];
  int pointpos;
};

template <int len>
void Ldouble<len>::setVal(const long a)
{
  data[pointpos - 1] = '0' + a;
  for (int i = 1; i < len + 2; i++)
  {
    data[i] = '0';
  }
  data[len + 1] = '\0';
}

template <int len>
void Ldouble<len>::plus(const Ldouble<len> &other)
{
  int sum;
  bool carrybit = false;
  // bit add bit
  for (int i = len; i >= 0; i--)
  {
    sum = (this->data[i] - '0') + (other.data[i] - '0') + carrybit;
    carrybit = sum > 9;
    this->data[i] = sum % 10 + '0';
  }
}

template <int len>
void Ldouble<len>::minus(const Ldouble<len> &other)
{
  int sum;
  bool carrybit = false;
  // bit minus bit
  for (int i = len; i >= 0; i--)
  {
    sum = 10 + (this->data[i] - '0') - (other.data[i] - '0') - carrybit;
    carrybit = sum < 10;
    this->data[i] = sum % 10 + '0';
  }
}

template <int len>
void Ldouble<len>::multi(const long m)
{
  int carrybit = 0;
  int sum;
  // bit multiply int
  for (int i = len; i >= 0; i--)
  {
    sum = (data[i] - '0') * m + carrybit;
    carrybit = sum / 10;
    data[i] = sum % 10 + '0';
  }
}

template <int len>
void Ldouble<len>::divi(const long m)
{
  // begin from 1 to len
  int remain = 0;
  for (int i = 0; i <= len; i++)
  {
    remain += data[i] - '0';
    data[i] = remain / m + '0';
    remain = remain % m * 10;
  }
}

template <int len>
void Ldouble<len>::print()
{
  // print long double value
  for (int i = 0; i < pointpos; i++)
  {
    printf("%c", data[i]);
  }

  printf(".");
  printf("%s\n", data + pointpos);
}

#endif // LDOUBLE_H