# Project1

> Author : 胡子昂 <br>
>
> 本次作业完成四种体系结构（openmp，mpi，pthread，ispc）对pi，矩阵乘法，矩阵转置的计算。

## 项目目录

- 项目目录如下：

~~~bash
$ tree -L 3 .
.
├── bin
│   ├── calpi
│   │   ├── calpi_ispc
│   │   ├── calpi_openmp
│   │   └── calpi_pthread
│   ├── multiplymatrix
│   │   ├── multiplymatrix_ispc
│   │   ├── multiplymatrix_openmp
│   │   └── multiplymatrix_pthread
│   └── transpositionmatrix
│       ├── transpositionmatrix_ispc
│       ├── transpositionmatrix_openmp
│       └── transpositionmatrix_pthread
├── inc
│   ├── calpi.h
│   ├── ldouble.h
│   ├── matrix.h
│   └── timer.h
└── src
    ├── ISPC
    │   ├── calpi
    │   ├── multiplymatrix
    │   └── transpositionmatrix
    ├── MPI
    │   ├── calpi.cpp
    │   ├── multiplymatrix.cpp
    │   └── transpositionmatrix.cpp
    ├── OpenMP
    │   ├── calpi.cpp
    │   ├── multiplymatrix.cpp
    │   └── transpositionmatrix.cpp
    └── pthread
        ├── calpi.cpp
        ├── multiplymatrix.cpp
        └── transpositionmatrix.cpp

13 directories, 22 files
~~~

（其中，bin是mac下的可执行程序，inc是头文件存放目录，src是源文件，按照四种体系结构放置）

## 项目编译运行环境

1. 所有程序都使用4个核处理。

2. 程序编译指令如下：

   ~~~bash
   # pthread
   $ g++ -std=c++11 -O3 -lpthread -o XXX XXX.cpp
   # openmp
   $ g++-8 -std=c++11 -O3 -fopenmp -o XXX XXX.cpp
   # mpi
   # ispc (见makefile)
   $ make
   ~~~

3. 程序数据的大小将在运行程序后输入。

## 机器的详细配置



## 程序运行结果

- 计算PI：

| 并行方法\数据大小 | 10000 |      |      |
| :---------------: | :---: | :--: | :--: |
|      pthread      |       |      |      |
|      openmp       |       |      |      |
|        MPI        |       |      |      |
|       ISPC        |       |      |      |

- 矩阵乘法：

| 并行方法\数据大小 |      |      |      |      |
| :---------------: | ---- | ---- | ---- | ---- |
|      pthread      |      |      |      |      |
|      openmp       |      |      |      |      |
|        MPI        |      |      |      |      |
|       ISPC        |      |      |      |      |

- 矩阵转置：

| 并行方法\数据大小 |      |      |      |
| :---------------: | ---- | ---- | ---- |
|      pthread      |      |      |      |
|      openmp       |      |      |      |
|        MPI        |      |      |      |
|       ISPC        |      |      |      |

## 实验结果

