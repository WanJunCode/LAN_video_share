// recv.c
//包含zmq的头文件
#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include "comm.h"

int main(int argc, char *argv[])
{
    void *pCtx = NULL;
    void *pSock = NULL;
    const char *pAddr = "ipc://drd.ipc";

    //创建context，zmq的socket 需要在context上进行创建
    if ((pCtx = zmq_ctx_new()) == NULL)
    {
        return 0;
    }
    //创建zmq socket ，socket目前有6种属性 ，这里使用dealer方式
    //具体使用方式请参考zmq官方文档（zmq手册）
    if ((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
    {
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    int iRcvTimeout = 5000; // millsecond
    //设置zmq的接收超时时间为5秒
    if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    char *pName = "recv";
    if (zmq_setsockopt(pSock, ZMQ_IDENTITY, pName, strlen(pName)) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    //绑定地址 ipc://drd.ipc
    //也就是使用ipc协议进行通信，地址为当前目录下的drd.ipc
    if (zmq_connect(pSock, pAddr) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    printf("bind at : %s\n", pAddr);
    ZmqSock zmqsock;
    zmqsock.iType = ZMQ_DEALER;
    zmqsock.sock = pSock;
    while (1)
    {
        printf("waitting...\n");
        errno = 0;
        //循环等待接收到来的消息，当超过5秒没有接到消息时，
        // recv函数返回错误信息 ，并使用zmq_strerror函数进行错误定位
        Zmqmsg zmsg;
        memset(&zmsg, 0, sizeof(zmsg));
        if (s_recv(&zmqsock, &zmsg) < 0)
        {
            printf("error = %s\n", zmq_strerror(errno));
            continue;
        }
        printf("------------------------\n");
    }

    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
}