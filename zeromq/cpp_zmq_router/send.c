// send.c
//包含zmq的头文件
#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include "comm.h"

int main(int argc, char *argv[])
{
    void *pCtx = NULL;
    void *pSock = NULL;
    //使用ipc协议进行通信，需要连接的目标机器IP地址为drd.ipc
    const char *pAddr = "ipc://drd.ipc";

    //创建context
    if ((pCtx = zmq_ctx_new()) == NULL)
    {
        return 0;
    }
    //创建socket
    if ((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
    {
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    int iSndTimeout = 5000; // millsecond
    //设置接收超时
    if (zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    char *pName = "send";
    if (zmq_setsockopt(pSock, ZMQ_IDENTITY, pName, strlen(pName)) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    if (zmq_connect(pSock, pAddr) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    printf("connect to [%s]\n", pAddr);
    ZmqSock zmqsock;
    zmqsock.iType = ZMQ_DEALER;
    zmqsock.sock = pSock;
    //循环发送消息
    while (1)
    {
        static int i = 0;
        Zmqmsg zmsg;
        memset(&zmsg, 0, sizeof(zmsg));
        snprintf(zmsg.szDst, sizeof(zmsg.szDst), "recv");
        snprintf(zmsg.szMsg, sizeof(zmsg.szMsg), "hello world : %3d", i++);
        printf("Enter to send...\n");
        if (s_send(&zmqsock, &zmsg) < 0)
        {
            fprintf(stderr, "send message faild\n");
        }
        printf("------------------------\n");
        getchar();
    }

    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
}