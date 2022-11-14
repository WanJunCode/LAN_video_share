#ifndef __ECHO_TASK__
#define __ECHO_TASK__

#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

class Hander
{
public:
    void operator()(int sock, std::string &ip, int port);
};

class Task
{
private:
    int sock;
    std::string ip;
    int port;
    Hander condut; //仿函数
public:
    Task() = default;
    Task(int _sock, std::string _ip, int _port)
        : sock(_sock), ip(_ip), port(_port)
    {
    }

    void Run()
    {
        condut(sock, ip, port);
    }
};

#endif