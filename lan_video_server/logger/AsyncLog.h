#pragma once

#include <assert.h>
#include <memory>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "FixedLenBuffer.h"

class AsyncLogging
{
public:
    AsyncLogging();

    AsyncLogging(const std::string &basename, off_t rollSize, int flushInterval = 3);

    ~AsyncLogging();

    void Init(const std::string &basename, off_t rollSize, int flushInterval = 3);

    void append(const char *logline, size_t len);

    void log(std::string log_msg);

    void start();

    void stop();

private:
    void threadFunc();

    typedef FixedLenBuffer<kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    int flushInterval_;
    std::atomic<bool> running_;
    std::string basename_;
    off_t rollSize_;
    std::thread thread_;
    CountDownLatch latch_;
    std::mutex mutex_;
    std::condition_variable cond_;

    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};