#include <glog/logging.h>
#include <gflags/gflags.h>

#include "Tcp_Server.h"
#include "LanVideoServer.h"
#include "LanDefine.h"

// Tcp_Sever +port

void User(char *name)
{
    std::cout << name << " + port" << std::endl;
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(LAN_LOG_PATH); // init glog
    google::SetLogDestination(google::GLOG_INFO, "../logs/INFO_");

    LOG(INFO) << "Start LAN VIDEO SERVER log";

    LanVideoServer lan_server(LAN_CONFIG_PATH);
    lan_server.Init();
    lan_server.Run();

    google::ShutdownGoogleLogging();
    return 0;
}
