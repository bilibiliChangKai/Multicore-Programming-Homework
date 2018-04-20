# Homework3

> Author : 胡子昂 <br>
>
> 本次作业完成多线程对pi的计算，使用mpi进行计算。

## 原理：

$$
\frac{\pi}{4} = 1 - \frac{1}{3} + \frac{1}{5} - ...
$$

- PS：虽然这种方法收敛性很低，但是适用多线程计算。
- 程序将分成多个线程分别计算相应部分的值，最后再进行规约求和。

## 程序结果：

用4核运行10000000次迭代如下：

~~~bash
$ mpirun -np 4 ./calpi
Process 1 of 4 on huziangdeMacBook-Pro.local
1 process enter calculate part!
1 process calculate [5000001, 10000001)!
Process 2 of 4 on huziangdeMacBook-Pro.local
2 process enter calculate part!
2 process calculate [10000001, 15000001)!
Process 3 of 4 on huziangdeMacBook-Pro.local
3 process enter calculate part!
3 process calculate [15000001, 20000001)!
Process 0 of 4 on huziangdeMacBook-Pro.local
0 process enter calculate part!
0 process calculate [1, 5000001)!
1 process over in 1.224599 times!
3 process over in 1.254090 times!
2 process over in 1.254074 times!
3.1415924496
0 process over in 1.250749 times!
~~~

