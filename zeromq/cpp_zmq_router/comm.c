// comm.c
#include <string.h>
#include "comm.h"

void lockSocket()
{
    // lock
}
void unlockSocket()
{
    // unlock
}

int s_recv(ZmqSock *zmqsock, Zmqmsg *zMsg)
{
    if (NULL == zmqsock || NULL == zMsg)
    {
        return -1;
    }
    int iRet = -1;
    lockSocket();
    int iType = 0;
    int len = sizeof(iType);
    void *sock = zmqsock->sock;
    do
    {
        iType = zmqsock->iType;
        if (ZMQ_ROUTER == iType)
        {
            printf("router:\n");
            errno = 0;
            if (zmq_recv(sock, zMsg->szSrc, sizeof(zMsg->szSrc), 0) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("recv : [%s]\n", zMsg->szSrc);
            if (zmq_recv(sock, NULL, 0, 0) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("recv : []\n");
        }
        else if (ZMQ_DEALER == iType)
        {
            printf("dealer:\n");
            if (zmq_recv(sock, NULL, 0, 0) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("recv : []\n");
        }
        else if (ZMQ_REQ == iType)
        {
            printf("req:\n");
        }
        else if (ZMQ_REP == iType)
        {
            printf("rep:\n");
        }

        if (zmq_recv(sock, zMsg->szDst, sizeof(zMsg->szDst), 0) < 0)
        {
            printf("send msg faild : [%s]\n", zmq_strerror(errno));
            break;
        }
        printf("recv : [%s]\n", zMsg->szDst);
        if (zmq_recv(sock, NULL, 0, 0) < 0)
        {
            printf("send msg faild : [%s]\n", zmq_strerror(errno));
            break;
        }
        printf("recv : []\n");
        if (zmq_recv(sock, zMsg->szMsg, sizeof(zMsg->szMsg), 0) < 0)
        {
            printf("send msg faild : [%s]\n", zmq_strerror(errno));
            break;
        }
        printf("recv : [%s]\n", zMsg->szMsg);
        iRet = 0;
    } while (0);
    unlockSocket();

    return iRet;
}

int s_send(ZmqSock *zmqsock, Zmqmsg *zMsg)
{
    if (NULL == zmqsock || NULL == zMsg)
    {
        return -1;
    }
    int iRet = -1;
    lockSocket();
    int iType = zmqsock->iType;
    int len = sizeof(iType);
    void *sock = zmqsock->sock;
    do
    {
        if (ZMQ_ROUTER == iType)
        {
            printf("router:\n");
            if (zmq_send(sock, zMsg->szDst, strlen(zMsg->szDst), ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : [%s]\n", zMsg->szDst);
            if (zmq_send(sock, "", 0, ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : []\n");
            if (zmq_send(sock, zMsg->szSrc, strlen(zMsg->szSrc), ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : [%s]\n", zMsg->szSrc);
        }
        else if (ZMQ_DEALER == iType)
        {
            printf("dealer:\n");
            if (zmq_send(sock, "", 0, ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : []\n");
            if (zmq_send(sock, zMsg->szDst, strlen(zMsg->szDst), ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : [%s]\n", zMsg->szDst);
        }
        else if (ZMQ_REQ == iType || ZMQ_REP == iType)
        {
            printf("rex:\n");
            if (zmq_send(sock, zMsg->szDst, strlen(zMsg->szDst), ZMQ_SNDMORE) < 0)
            {
                printf("send msg faild : [%s]\n", zmq_strerror(errno));
                break;
            }
            printf("send : [%s]\n", zMsg->szDst);
        }

        if (zmq_send(sock, "", 0, ZMQ_SNDMORE) < 0)
        {
            printf("send msg faild : [%s]\n", zmq_strerror(errno));
            break;
        }
        printf("send : []\n");
        if (zmq_send(sock, zMsg->szMsg, strlen(zMsg->szMsg), 0) < 0)
        {
            printf("send msg faild : [%s]\n", zmq_strerror(errno));
            break;
        }
        printf("send : [%s]\n", zMsg->szMsg);
        iRet = 0;
    } while (0);
    unlockSocket();

    return iRet;
}