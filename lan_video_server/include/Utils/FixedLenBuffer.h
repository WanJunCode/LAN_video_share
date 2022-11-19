#pragma once

#include <assert.h>
#include <memory>
#include <string.h>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <size_t LENGTH>
class FixedLenBuffer
{
public:
    FixedLenBuffer()
    {
        memset(data, 0, LENGTH);
        current_positon = data;
    }
    ~FixedLenBuffer()
    {
    }

    size_t AvailableSize()
    {
        return data + LENGTH - current_positon;
    }

    size_t Size()
    {
        return current_positon - data;
    }

    char *Current()
    {
        return current_positon;
    }

    void Append(const char *buf, size_t len)
    {
        if (static_cast<size_t>(AvailableSize()) > len)
        {
            memcpy(current_positon, buf, len);
            current_positon += len;
        }
    }

    void Bzero()
    {
        memset(data, 0, LENGTH);
    }

    const char *Data() const { return data; }

    void Reset()
    {
        current_positon = data;
    }

private:
    char *current_positon;
    char data[LENGTH];
};

class AppendFile
{
public:
    explicit AppendFile(std::string filename) : fp_(::fopen(filename.c_str(), "ae"))
    {
        // assert(fp_);
        ::setbuffer(fp_, buffer_, sizeof buffer_);
    }

    ~AppendFile()
    {
        ::fclose(fp_);
    }

    void append(const char *logline, size_t len)
    {
        size_t written = 0;

        while (written != len)
        {
            size_t remain = len - written;
            size_t n = write(logline + written, remain);
            if (n != remain)
            {
                int err = ferror(fp_);
                if (err)
                {
                    fprintf(stderr, "AppendFile::append() failed\n");
                    break;
                }
            }
            written += n;
        }

        writtenBytes_ += written;
    }

    void flush()
    {
        ::fflush(fp_);
    }

    off_t writtenBytes() const { return writtenBytes_; }

private:
    size_t write(const char *logline, size_t len)
    {
        return ::fwrite_unlocked(logline, 1, len, fp_);
    }

    FILE *fp_;
    char buffer_[64 * 1024];
    off_t writtenBytes_ = {0};
};

class LogFile
{
public:
    LogFile(const std::string &basename,
            off_t rollSize,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    ~LogFile();

    void append(const char *logline, int len);

    void flush();

    bool rollFile();

private:
    void append_unlocked(const char *logline, int len);

    static std::string getLogFileName(const std::string &basename, time_t *now);

    const std::string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveryN_;

    int count_;

    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<AppendFile> append_file_;

    const static int kRollPerSeconds_ = 60 * 60 * 24;
};

class CountDownLatch
{
public:
    explicit CountDownLatch(int count) : count_(count)
    {
    }

    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ > 0)
        {
            condition_.wait(lock);
        }
    }

    void countDown()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        --count_;
        if (count_ == 0)
        {
            condition_.notify_all();
        }
    }

    int getCount() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    int count_;
};

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