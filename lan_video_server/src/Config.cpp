#include "Config.h"

#include <json/json.h>
#include <iostream>
#include <glog/logging.h>

LanConfig::LanConfig()
{
}

LanConfig::~LanConfig()
{
}

void LanConfig::Init(std::string path)
{
    LOG(INFO) << "LanConfig init config " << path;
    config_file_path = path;
}