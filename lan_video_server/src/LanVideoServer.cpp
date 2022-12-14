#include "LanVideoServer.h"

#include <glog/logging.h>
#include <functional>

#include "Config.h"
#include "Singleton.h"
#include "Tcp_Server.h"

#include "LanPublisher.h"
#include "LanRequest.h"
#include "LanRequestPubSubTypes.h"

#include "AsyncLog.h"
#include "LanLogger.h"

LanVideoServer::LanVideoServer(std::string path) : config_path(path)
{
}

LanVideoServer::~LanVideoServer()
{
    if (run_thread.joinable())
    {
        printf("wait for run_thread finish\n");
        run_thread.join();
    }

    if (dds_thread.joinable())
    {
        printf("wait for dds_thread finish\n");
        dds_thread.join();
    }
}

void LanVideoServer::Init()
{
    off_t kRollSize = 500*1000*1000;
    TemPlateSingleton<AsyncLogging>::GetInstance()->Init("lan_file", kRollSize, 3);
    TemPlateSingleton<AsyncLogging>::GetInstance()->start();
    TemPlateSingleton<LanConfigManager>::GetInstance()->Init(config_path);

    // logger 单例
    TemPlateSingleton<LanLogger>::GetInstance()->debug();
    TemPlateSingleton<LanLogger>::GetInstance()->Init();
    TemPlateSingleton<LanLogger>::GetInstance()->debug();
}

void LanVideoServer::Run()
{
    run_thread = std::thread(LanVideoServer::listen_process);
    dds_thread = std::thread(LanVideoServer::dds_publish);
}

void LanVideoServer::listen_process()
{
    printf(">>>>>>>>>>>> start listen_process\n");
    TcpSever *Server = new TcpSever(123); //将字符串形式的端口号转化为整数形式
    Server->InitSever();
    Server->Loop();
}

void LanVideoServer::dds_publish()
{
    printf(">>>>>>>>>>>> begin start dds publish\n");
    LanPublisher<lan_vodeo::LanRequest, lan_vodeo::LanRequestPubSubType> lan_publisher;
    if (lan_publisher.init("PersistentLanRequestTopic"))
    {
        lan_publisher.run();
    }
}
