#pragma once

#include <mutex>
#include <condition_variable>

// TODO 是否需要使用异常？？？

// 异常类
class latch_broken_exception {
};

class countdown_latch
{
private:
    volatile int count;
    std::mutex mut;
    std::condition_variable cd_condition_var;

public:
    countdown_latch(int count);
    ~countdown_latch();

    // 禁止赋值、复制构造函数
    countdown_latch(const countdown_latch &) = delete;
    countdown_latch &operator=(const countdown_latch &) = delete;

    int get_count()
    {
        return count;
    }
    void wait();
    void countdown();
    void interrupt();
};

countdown_latch::countdown_latch(int cnt) : count(cnt)
{
    if (count < 0)
    {
        throw std::string("parameter error!");
    }
}

countdown_latch::~countdown_latch()
{
    std::lock_guard<std::mutex> lg(mut);
    if (count > 0)
    { // 如果counte>0，说明还有正在等待的线程，抛出异常
        throw std::string("countdown_latch state error!");
    }
}

void countdown_latch::wait()
{
    if (count > 0)
    {
        // 双检查法，如果count等于0，说明已经倒计数到0，不用等待了
        std::unique_lock<std::mutex> ul(mut);
        cd_condition_var.wait(ul, [this]
                { return count <= 0; });
        if (count == -1)
        { // 如果是被中断的，抛出异常
            throw latch_broken_exception();
        }
    }
}

void countdown_latch::countdown()
{
    if (count > 0)
    { // 双检查法，第一次检查
        std::lock_guard<std::mutex> lg(mut);
        if (count > 0)
        { // 第二次检查在mutex保护范围内
            --count;
            if (count == 0)
            { // 倒计数为0，唤醒等待中的线程
                cd_condition_var.notify_all();
            }
        }
    }
}

void countdown_latch::interrupt()
{ // 当中断时，如果还有正在waiting的线程，唤醒它们
    if (count <= 0)
    { // 此处无需使用release语义，如果 count <= 0，说明已经被别的线程唤醒了，在那里会有一个release语义，如果count > 0，由下面的mutex在unlock时来保证release
        return;
    }

    std::lock_guard<std::mutex> lg(mut); // mutex本身就提供了release语义，此函数无需提供专门的release语义
    if (count > 0)
    { // 当count = 0时，则说明已经倒计数到0了，不用发送中断通知了
        count = -1;
    }
    cd_condition_var.notify_all();
}
