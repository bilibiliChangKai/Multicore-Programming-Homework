// Author : huziang
// Date : 2018年4月23日 星期一
//   this is a client, send data to server

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT 7124
#define BUFFER_SIZE 1024

char recvbuf[BUFFER_SIZE];
char sendbuf[BUFFER_SIZE];

int main()
{
    int socketclient;
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    // 定义套接字
    socketclient = socket(AF_INET, SOCK_STREAM, 0);

    // 设置套接字
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);                 // server host
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // server ip

    // 连接服务器，成功返回0，错误返回-1
    while (connect(socketclient, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    printf("> ");
    fflush(stdout);

    while (1)
    {
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rfds);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(socketclient, &rfds);
        /*找出文件描述符集合中最大的文件描述符*/
        if (maxfd < socketclient)
        {
            maxfd = socketclient;
        }
        /*设置超时时间*/
        tv.tv_sec = 100;
        tv.tv_usec = 0;

        /*等待聊天*/
        retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            printf("select出错，客户端程序退出\n");
            break;
        }
        else if (retval == 0)
        {
            printf("客户端没有任何输入信息，并且服务器也没有信息到来，waiting...\n");
            continue;
        }
        else
        {
            /*服务器发来了消息*/
            if (FD_ISSET(socketclient, &rfds))
            {
                int len;
                len = recv(socketclient, recvbuf, sizeof(recvbuf), 0);
                recvbuf[len] = '\0';
                printf("S: %s\n", recvbuf);
                printf("> ");
                fflush(stdout);
            }

            /*用户输入信息了,开始处理信息并发送*/
            if (FD_ISSET(0, &rfds))
            {
                printf("> ");
                fflush(stdout);

                memset(sendbuf, '0', strlen(sendbuf));
                fgets(sendbuf, sizeof(sendbuf), stdin);
                if (strlen(sendbuf) == 0)
                {
                    continue;
                }

                // 去除'\n'
                sendbuf[strlen(sendbuf) - 1] = '\0';
                send(socketclient, sendbuf, strlen(sendbuf), 0); //发送
            }
        }
    }

    close(socketclient);
    return 0;
}