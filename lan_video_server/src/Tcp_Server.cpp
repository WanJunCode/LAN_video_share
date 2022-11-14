#include "Tcp_Server.h"

TcpSever::TcpSever(int _port) : port(_port), listen_sock(-1), Pool(nullptr)
{
}

void TcpSever::InitSever()
{
    listen_sock = socket(AF_INET, SOCK_STREAM, 0); // IPV4流式套接
    if (listen_sock < 0)
    {
        std::cerr << "监听套接字创建失败" << std::endl;
        exit(-1);
    }
    //绑定
    struct sockaddr_in local;
    memset(&local, '\0', sizeof(local)); //初始化结构体
    local.sin_family = AF_INET;
    local.sin_port = htons(port);       //主机转网络
    local.sin_addr.s_addr = INADDR_ANY; //随机绑定服务器ip
    if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    {
        std::cerr << "绑定失败" << std::endl;
        exit(-2);
    }
    //建立链接
#define BACK_LOG 5
    if (listen(listen_sock, BACK_LOG) < 0)
    {
        std::cerr << "链接失败" << std::endl;
        exit(-3);
    }
    std::cout << "服务启动成功" << std::endl;
    Pool = new ThreadPool<Task>(10); // 10个线程
}

//获取链接
void TcpSever::Loop()
{
    Pool->InitThreadPool();
    //获取客户端信息
    struct sockaddr_in client;
    // signal(SIGCHID,hander);//hander是信号捕捉方法
    while (true)
    {
        socklen_t len = sizeof(client);
        //获取新链接
        int sock = accept(listen_sock, (struct sockaddr *)&client, &len);
        if (sock < 0)
        {
            std::cerr << "接受链接失败" << std::endl;
            continue; //继续接受其他链接请求
        }
        std::string ip = inet_ntoa(client.sin_addr);
        int port = ntohs(client.sin_port);
        Task asig(sock, ip, port);
        Pool->Push(asig);
    }
}

TcpSever::~TcpSever()
{
    if (listen_sock >= 0)
    {
        close(listen_sock);
    }
}