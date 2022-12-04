异步日志库
参考 muduo

FixedLenBuffer 模板类
内部维护一个连续的char data[SIZE] 内存段

AppendFile 打开指定的目录文件，并向文件中append追加日志
内部有一个 64K 大小的文件写入缓冲区，主要通过函数 setbuffer 设置
该类使用 fwrite_unlocked 写入数据到文件中，该写入操作是线程不安全的

LogFile

CountDownLatch  倒数计时器
主要用于多个线程之间有需要准备的操作，等到所有线程完成处理后才会继续向下执行