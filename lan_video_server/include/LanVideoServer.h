#pragma once

#include <string>

#include "Config.h"

class LanVideoServer
{
private:
    std::string config_path;
public:
    LanVideoServer(std::string path);
    ~LanVideoServer();

    void Init();
    void Run();
};

