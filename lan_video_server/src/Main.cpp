#include <glog/logging.h>
#include <gflags/gflags.h>
#include <chrono>

#include "LanVideoServer.h"
#include "LanDefine.h"

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(LAN_LOG_PATH); // init glog
    google::SetLogDestination(google::GLOG_INFO, "../logs/INFO_");

    LOG(INFO) << "Start LAN VIDEO SERVER log";

    LanVideoServer lan_server(LAN_CONFIG_PATH);
    lan_server.Init();
    lan_server.Run();

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        printf("main sleep\n");
    }

    google::ShutdownGoogleLogging();
    return 0;
}
