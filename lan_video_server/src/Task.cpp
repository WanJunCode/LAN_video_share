#include "Task.h"

#include <json/json.h>

#include "Timer.h"
#include "FixedLenBuffer.h"
#include "Singleton.h"

static std::string ConvertJson2Str(Json::Value &root)
{
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> json_write(writerBuilder.newStreamWriter());
    std::ostringstream ss;
    json_write->write(root, &ss);
    return ss.str();
}

static std::string ProcessJsonMessage(std::string req_str)
{
    Json::Reader jsonReader;
    Json::Value jsonValue;
    if (!jsonReader.parse(req_str, jsonValue, true))
    {
        return "read error";
    }

    if (!jsonValue.isMember("name"))
    {
        return "no name member";
    }

    if (!jsonValue.isMember("value"))
    {
        return "no value member";
    }

    Json::Value test = jsonValue["name"];
    auto name = test.asString();
    auto value = jsonValue["value"].asString();
    jsonValue["name"] = "after process " + name;
    jsonValue["value"] = "after process " + value;

    TemPlateSingleton<AsyncLogging>::GetInstance()->log("receiver message from client " + name);
    return ConvertJson2Str(jsonValue);
}

void Hander::operator()(int sock, std::string &ip, int port)
{
    char buff[1024];
    while (true)
    {
        memset(buff, 0, sizeof(buff));
        size_t ReadSize = read(sock, buff, sizeof(buff) - 1);
        if (ReadSize > 0)
        {
            buff[ReadSize] = '\0'; //添加字符串结束标志
            std::cout << "Client[" << ip << "-port-" << port << "]# " << buff << std::endl;

            // 需求返回响应前加时间
            std::string serverResponse = GetCurrentTimeStamp() + " : " + ProcessJsonMessage(buff);
            write(sock, serverResponse.c_str(), serverResponse.size()); //向客户端写入数据
        }
        else if (ReadSize == 0)
        { //对端关闭链接
            std::cout << "Client[" << ip << "-port-" << port << "]# 关闭" << std::endl;
            break;
        }
        else
        {
            std::cerr << sock << "读取错误" << std::endl;
            break;
        }
    }
    std::cout << "服务终止" << std::endl;
    close(sock);
}