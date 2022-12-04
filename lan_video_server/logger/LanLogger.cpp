#include "LanLogger.h"

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"

void LanLogger::debug()
{
    printf("LanLogger debug\n");
    if (async_file) {
        async_file->info("logger spdlog test");
    }
}

void LanLogger::Init()
{
    spdlog::set_pattern("[%H:%M:%S:%e][%P][%t][%L] %v");
    spdlog::flush_every(std::chrono::seconds(1));
    spdlog::init_thread_pool(8192, 1);
    auto max_size = 1048576 * 100;
    auto fileName = "./logs/lan_server.log";
    auto fileCount = 3;
    try
    {
        async_file = spdlog::rotating_logger_mt<spdlog::async_factory>("lan_server", fileName, max_size, fileCount);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}