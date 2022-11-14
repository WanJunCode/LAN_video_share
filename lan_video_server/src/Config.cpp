#include "Config.h"

#include <json/json.h>
#include <iostream>

LanConfig::LanConfig()
{
}

LanConfig::~LanConfig()
{
}

void LanConfig::Init(std::string path)
{
    config_file_path = path;
    std::cout << "demo test" << std::endl;
}