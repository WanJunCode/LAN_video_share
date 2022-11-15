#include "Config.h"

#include <json/json.h>
#include <iostream>
#include <glog/logging.h>

LanConfigManager::LanConfigManager()
{
}

LanConfigManager::~LanConfigManager()
{
}

void LanConfigManager::Init(std::string path)
{
    LOG(INFO) << "LanConfigManager init config " << path;
    config_file_path = path;
}