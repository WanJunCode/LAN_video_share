//comm.h
#ifndef _ZMQCOMM_H_
#define _ZMQCOMM_H_
#include <zmq.h>

#define NAME_LEN        256
#define MSG_LEN         1024

typedef struct {
    char szSrc[NAME_LEN];
    char szDst[NAME_LEN];
    char szMsg[MSG_LEN];
}Zmqmsg;

typedef struct {
    void * sock;
    int iType;
}ZmqSock;

void lockSocket();
void unlockSocket();

int  s_recv(ZmqSock * sock, Zmqmsg * zMsg);
int  s_send(ZmqSock * sock, Zmqmsg * zMsg);

#endif