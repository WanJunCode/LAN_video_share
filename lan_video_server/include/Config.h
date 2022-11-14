#pragma once

#include <string>

class LanConfig {
public:
    LanConfig();
    ~LanConfig();
    void Init(std::string path);

private:
    std::string config_file_path;
};