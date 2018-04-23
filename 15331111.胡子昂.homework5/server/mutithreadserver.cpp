// Author : huziang
// Date : 2018年4月23日 星期一
//   this is a server, recieve data and then distribute
// data to working thread

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
#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
using std::cin;
using std::cout;
using std::endl;
using std::string;

#define PORT 7124
#define QUEUE_NUM 20
#define BUFFER_SIZE 1024
#define THREAD_NUM 4
#define SLEEP_TIME 5

pthread_t pids[THREAD_NUM + 1];
pthread_mutex_t semmutex;
pthread_cond_t semcond;
volatile int busynum = THREAD_NUM;
string data;

char recvbuffer[BUFFER_SIZE];
char sendbuffer[BUFFER_SIZE];
int conn;   // 用于线程发送数据

// using tcp recieve data
void *distributeThread(void *argv) {
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;

    /*创建socket*/
    int ss = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET   IPV4   ;SOCK_STREAM   TCP
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*bind*/
    if (bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    /*listen*/
    if(listen(ss, QUEUE_NUM) == -1) {
        perror("listen");
        exit(1);
    }

    /*connect*/
    struct sockaddr_in client_addr;
    socklen_t client_length = sizeof(client_addr);
    // 成功返回非负描述字，出错返回-1
    conn = accept(ss, (struct sockaddr*)&client_addr, &client_length);   //目测需要客户端部分的addr
    if( conn < 0 ) {
        perror("connect");
        exit(1);
    }

    while(1) {
        /*把可读文件描述符的集合清空*/
        FD_ZERO(&rfds);
        /*把标准输入的文件描述符加入到集合中*/
        FD_SET(0, &rfds);
        maxfd = 0;
        /*把当前连接的文件描述符加入到集合中*/
        FD_SET(conn, &rfds);
        /*找出文件描述符集合中最大的文件描述符*/
        if (maxfd < conn) {
            maxfd = conn;
        }
        /*设置超时时间*/
        tv.tv_sec = 100;
        tv.tv_usec = 0;
        /*等待聊天*/

        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            printf("select出错，客户端程序退出\n");
            break;
        } else if (retval == 0){
            printf("服务端没有任何输入信息，并且客户端也没有信息到来，waiting...\n");
            continue;
        } else {
            /*客户端发来了消息*/
            if (FD_ISSET(conn,&rfds)) {
                memset(recvbuffer, '\0', strlen(recvbuffer));
                int len = recv(conn, recvbuffer, sizeof(recvbuffer), 0);
                // printf("C: %s\n", recvbuffer);
                
                // 无效信息，退出
                if (len == 0) {
                    break;
                }

                // 交给线程处理
                if (busynum == THREAD_NUM) {
                    strcpy(sendbuffer, "******* Error: All thread is busy!");
                    cout << sendbuffer << endl;
                    send(conn, sendbuffer, strlen(sendbuffer) , 0); // 把数据回发给客户端
                } else {
                    pthread_mutex_lock(&semmutex);
                    string temp(recvbuffer);
                    data = temp;
                    pthread_mutex_unlock(&semmutex);
                    pthread_cond_signal(&semcond);
                }
            }
        }
    }

    close(conn);
    close(ss);
    exit(0);
}

// working thread, deal with recieve data
void *workingThread(void *argv) {
    // get id from address
    long id = (long)argv;
    string mydata;

    while (1) {
        pthread_mutex_lock(&semmutex);
        // critical area
        send(conn, mydata.c_str(), mydata.length(), 0);
        busynum--;
        // wait signal
        pthread_cond_wait(&semcond, &semmutex);
        // critical area
        busynum++;
        mydata.clear();
        mydata = data;
        pthread_mutex_unlock(&semmutex);

        // deal with data
        cout << "Thread " << id << " begin!" << endl;
        cout << "Thread " << id << " is working with the data \"" << mydata << "\"!" << endl;
        sleep(SLEEP_TIME);
        cout << "Thread " << id << " end!" << endl;
        mydata = string("Thread ") + char(id + '0') + " deal with your data \"" + mydata + "\"!";
        //cout << mydata << endl;
    }
}

int main() {
    // init mutex
    pthread_mutex_init(&semmutex, NULL);
    pthread_cond_init(&semcond, NULL);

    // create distribute thread
    pthread_create(&pids[0], NULL, distributeThread, NULL);

    // create working thread
    for (long i = 1; i <= THREAD_NUM; ++i) {
        void *argv = (void*)i;
        pthread_create(&pids[i], NULL, workingThread, argv);
    }

    // join
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(pids[i], NULL);
    }

    // destroy mutex
    pthread_mutex_destroy(&semmutex);
    pthread_cond_destroy(&semcond);
}