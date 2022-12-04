#pragma once

#include <stdio.h>
#include <memory>

namespace spdlog {
    class logger;
}

class LanLogger {
public:
    LanLogger() {
        printf("construct Lanlogger\n");
    }

    void Init();

    void debug();

private:
    std::shared_ptr<spdlog::logger> async_file;
};