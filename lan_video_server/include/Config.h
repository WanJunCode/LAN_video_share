#pragma once

#include <string>

class LanConfig
{
public:
    std::string listen_ip = "127.0.0.1";
    unsigned int listen_port = 123;
    std::string service_type = "default";
};

class LanConfigManager
{
public:
    LanConfigManager();
    ~LanConfigManager();
    void Init(std::string path);

private:
    std::string config_file_path;
    LanConfig lan_config_item;
};