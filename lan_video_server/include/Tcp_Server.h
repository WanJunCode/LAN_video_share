#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>

#include "ThreadPool.h"
#include "Task.h"

#define DEF_PORT 8081

//通信结构体
class Pragram
{
public:
	int sock;
	std::string ip;
	int port;

	Pragram(int _sock, std::string _ip, int _port)
		: sock(_sock), ip(_ip), port(_port)
	{
	}
};

class TcpSever
{
private:
	int port;
	int listen_sock; //监听套接字
	ThreadPool<Task> *Pool;

public:
	TcpSever(int _port = DEF_PORT);

	void InitSever();

	//获取链接
	void Loop();

	~TcpSever();
};
