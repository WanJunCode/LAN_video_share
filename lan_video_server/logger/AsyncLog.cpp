#include "AsyncLog.h"

#include <functional>

#include "LogFile.h"

AsyncLogging::AsyncLogging() : latch_(1), running_(false){

}

AsyncLogging::AsyncLogging(const std::string &basename,
                           off_t rollSize,
                           int flushInterval)
    : flushInterval_(flushInterval),
      basename_(basename),
      rollSize_(rollSize),
      latch_(1),
      running_(false)
{
    currentBuffer_ = std::make_unique<Buffer>();
    nextBuffer_ = std::make_unique<Buffer>();
    currentBuffer_->Bzero();
    nextBuffer_->Bzero();
    buffers_.reserve(16);
}

AsyncLogging::~AsyncLogging()
{
    if (running_)
    {
        stop();
    }
}

void AsyncLogging::Init(const std::string &basename, off_t rollSize, int flushInterval) {
    basename_ = basename;
    rollSize_ = rollSize;
    flushInterval_ = flushInterval;

    currentBuffer_ = std::make_unique<Buffer>();
    nextBuffer_ = std::make_unique<Buffer>();
    currentBuffer_->Bzero();
    nextBuffer_->Bzero();
    buffers_.reserve(16);
}

void AsyncLogging::start()
{
    running_ = true;
    thread_ = std::thread(std::bind(&AsyncLogging::threadFunc, this));
    latch_.wait();
}

void AsyncLogging::stop()
{
    running_ = false;
    cond_.notify_one();

    if (thread_.joinable())
    {
        thread_.join();
    }
}

void AsyncLogging::append(const char *logline, size_t len)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (currentBuffer_->AvailableSize() > len)
    {
        currentBuffer_->Append(logline, len);
    }
    else
    {
        buffers_.push_back(std::move(currentBuffer_));

        if (nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);
        }
        else
        {
            currentBuffer_.reset(new Buffer); // Rarely happens
        }
        currentBuffer_->Append(logline, len);
        cond_.notify_one();
    }
}

void AsyncLogging::log(std::string log_msg)
{
    append(log_msg.c_str(), log_msg.length());
}

void AsyncLogging::threadFunc()
{
    assert(running_ == true);
    latch_.countDown();
    
    LogFile output(basename_, rollSize_, false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->Bzero();
    newBuffer2->Bzero();

    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while (running_)
    {
        assert(newBuffer1 && newBuffer1->Size() == 0);
        assert(newBuffer2 && newBuffer2->Size() == 0);
        assert(buffersToWrite.empty());

        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (buffers_.empty()) // unusual usage!
            {
                cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
            }
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(newBuffer1);
            buffersToWrite.swap(buffers_);
            if (!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2);
            }
        }

        assert(!buffersToWrite.empty());

        // 待写入的缓存块太多时
        if (buffersToWrite.size() > 25)
        {
            char buf[256];
            // snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
            //          Timestamp::now().toFormattedString().c_str(),
            //          buffersToWrite.size() - 2);
            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            // 删到只剩两个
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for (const auto &buffer : buffersToWrite)
        {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            output.append(buffer->Data(), buffer->Size());
        }

        // 已经将buffer数组全部落盘后，只需要复用两个buffer
        if (buffersToWrite.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->Reset(); // 复用 buffer
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->Reset(); // 复用 buffer
        }

        buffersToWrite.clear();

        // 文件落盘刷新
        output.flush();
    }

    // 异步日志退出时最后会刷新 落盘一次
    output.flush();
}