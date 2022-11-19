#include "Utils/FixedLenBuffer.h"
#include "Utils/Utils.h"

#include <thread>
#include <functional>

LogFile::LogFile(const std::string &basename,
                 off_t rollSize,
                 bool threadSafe,
                 int flushInterval,
                 int checkEveryN)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      count_(0),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0)
{
    // assert(basename.find('/') == std::string::npos);
    rollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const char *logline, int len)
{
    append_unlocked(logline, len);
}

void LogFile::flush()
{
    append_file_->flush();
}

void LogFile::append_unlocked(const char *logline, int len)
{
    append_file_->append(logline, len);

    if (append_file_->writtenBytes() > rollSize_)
    {
        rollFile();
    }
    else
    {
        ++count_;
        if (count_ >= checkEveryN_)
        {
            count_ = 0;
            time_t now = ::time(NULL);
            time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
            if (thisPeriod_ != startOfPeriod_)
            {
                rollFile();
            }
            else if (now - lastFlush_ > flushInterval_)
            {
                lastFlush_ = now;
                append_file_->flush();
            }
        }
    }
}

bool LogFile::rollFile()
{
    time_t now = 0;
    std::string filename = getLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

    if (now > lastRoll_)
    {
        lastRoll_ = now;
        lastFlush_ = now;
        startOfPeriod_ = start;
        append_file_.reset(new AppendFile(filename));
        return true;
    }
    return false;
}

std::string LogFile::getLogFileName(const std::string &basename, time_t *now)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm); // FIXME: localtime_r ?
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;

    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%u", PthreadSelf());
    filename += pidbuf;

    filename += ".log";

    return filename;
}

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