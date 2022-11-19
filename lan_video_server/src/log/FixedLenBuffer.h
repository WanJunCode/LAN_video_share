#pragma once

#include <string.h>
#include <atomic>
#include <mutex>
#include <condition_variable>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <size_t FIXED_SIZE>
class FixedLenBuffer
{
public:
    FixedLenBuffer()
    {
        memset(data, 0, FIXED_SIZE);
        current_positon = data;
    }
    ~FixedLenBuffer()
    {
    }

    size_t AvailableSize()
    {
        return data + FIXED_SIZE - current_positon;
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
        memset(data, 0, FIXED_SIZE);
    }

    const char *Data() const { return data; }

    void Reset()
    {
        current_positon = data;
    }

private:
    char *current_positon;
    char data[FIXED_SIZE];
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
    volatile int count_;
};

