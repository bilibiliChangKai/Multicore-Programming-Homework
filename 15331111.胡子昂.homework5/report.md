#Homework5

> Author : huziang

## 程序说明

程序分为两部分，服务端和客户端，使用tcp连接：

- 客户端：客户端发送数据给服务端，按ctrl+c后同时终止服务端和客户端。
- 服务端：接收客户端数据，由分发线程分发任务。
  - 分发线程：将客户端数据传输给工作线程，如果工作线程已满，返回报错信息。
  - 工作线程：显示提示信息后睡眠5s，之后将结果发送回客户端。

## 程序样例输出：

- 客户端：

  ~~~bash
  $ ./mutithreadclient 
  > data1
  > data2
  > data3
  > data4
  > S: Thread 2 deal with your data "data1"!
  > S: Thread 1 deal with your data "data2"!
  > S: Thread 3 deal with your data "data3"!
  > S: Thread 4 deal with your data "data4"!
  > 1
  > 2
  > 3
  > 4
  > 5
  > S: ******* Error: All thread is busy!
  > 6
  > S: ******* Error: All thread is busy!
  > 7
  > S: ******* Error: All thread is busy!
  > S: Thread 2 deal with your data "1"!
  > S: Thread 1 deal with your data "2"!
  > S: Thread 3 deal with your data "3"!
  > S: Thread 4 deal with your data "4"!
  ~~~

- 服务端：

  ~~~bash
  $ ./mutithreadserver
  Thread 2 begin!
  Thread 2 is working with the data "data1"!
  Thread 1 begin!
  Thread 1 is working with the data "data2"!
  Thread 3 begin!
  Thread 3 is working with the data "data3"!
  Thread 4 begin!
  Thread 4 is working with the data "data4"!
  Thread 2 end!
  Thread 1 end!
  Thread 3 end!
  Thread 4 end!
  Thread 2 begin!
  Thread 2 is working with the data "1"!
  Thread 1 begin!
  Thread 1 is working with the data "2"!
  Thread 3 begin!
  Thread 3 is working with the data "3"!
  Thread 4 begin!
  Thread 4 is working with the data "4"!
  ******* Error: All thread is busy!
  ******* Error: All thread is busy!
  ******* Error: All thread is busy!
  Thread 2 end!
  Thread 1 end!
  Thread 3 end!
  Thread 4 end!
  ~~~

  ​