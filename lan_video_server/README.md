2022/11/13
完成django和C++后台程序框架的搭建，联通tcp客户端请求及服务端响应的回显在浏览器上
cmake框架搭建完成


cmake
message([<mode>] "message text" ...)
mode 的值包括 FATAL_ERROR、WARNING、AUTHOR_WARNING、STATUS、VERBOSE等。我主要使用其中的 2 个——FATAL_ERROR、STATUS

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/code/LAN_video_share/lan_video_server/lib

日志库选型
glog
log4cplus
spdlog

2022/12/4
spdlog只需要引入头文件即可使用

使用include、src的方式存储代码，相关模块不能好的体现。现在开始划分不同的模块类型并创建文件夹
logger模块用于封装日志的使用