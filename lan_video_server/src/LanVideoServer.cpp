#include "LanVideoServer.h"
#include "Config.h"
#include "Singleton.h"
#include "Tcp_Server.h"

LanVideoServer::LanVideoServer(std::string path) : config_path(path)
{
}

LanVideoServer::~LanVideoServer()
{
}

void LanVideoServer::Init()
{
    TemPlateSingleton<LanConfig>::GetInstance()->Init(config_path);
}

void LanVideoServer::Run()
{
    TcpSever *Server = new TcpSever(123); //将字符串形式的端口号转化为整数形式
    Server->InitSever();
    Server->Loop();
}