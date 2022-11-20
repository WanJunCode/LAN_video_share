#pragma once

#include <string>
#include <thread>
#include <memory>

#include "Config.h"

class LanVideoServer
{
private:
    std::string config_path;
    std::thread run_thread;
    std::thread dds_thread;

public:
    LanVideoServer(std::string path);
    ~LanVideoServer();

    void Init();
    void Run();
    static void listen_process();
    static void dds_publish();
};