#include "Tcp_Server.h"

// Tcp_Sever +port

void User(char *name)
{
    std::cout << name << " + port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        User(argv[0]);
        exit(-1);
    }
    TcpSever *Server = new TcpSever(atoi(argv[1])); //将字符串形式的端口号转化为整数形式
    Server->InitSever();
    Server->Loop();
    delete Server;
    return 0;
}
