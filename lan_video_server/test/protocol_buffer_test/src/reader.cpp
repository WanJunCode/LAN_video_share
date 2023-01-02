// reader.cpp
#include <iostream>
#include <string>
#include <fstream>
#include "my.pb.h"
using namespace std;

void ListMsg(const my::helloworld &msg)
{
    cout << msg.id() << endl;
    cout << msg.str() << endl;
}

int main(int argc, char *argv[])
{

    my::helloworld msg1;

    {
        fstream input("./hhh.txt", ios::in | ios::binary);
        if (!msg1.ParseFromIstream(&input))
        {
            cout << "Failed to parse address book." << endl;
            return -1;
        }
        ListMsg(msg1);
    }

    {
        std::string tmpStr;
        if (!msg1.SerializeToString(&tmpStr)) {
            cout << "Failed to serialize to string." << endl;
            return -1;
        } else {
            cout << "Success serialize to string: " << tmpStr << endl;
        }

        my::helloworld msg2;
        if (!msg2.ParseFromString(tmpStr)) {
            cout << "Failed to parse from string." << endl;
            return -1;
        } else {
            cout << "Success parse from string." << endl;
            ListMsg(msg2);
        }
    }

    // pb 结构体序列化为 std::string ===== SerializeToString        参数为 std::string 指针
    // std::string 反序列化为 pd 结构体 ===== ParseFromString

    {
        char tmpBuffer[1024];
        memset(tmpBuffer, 0, sizeof tmpBuffer);


        if (!msg1.SerializeToArray(tmpBuffer, sizeof tmpBuffer)) {
            cout << "Failed to serialize to string." << endl;
            return -1;
        } else {
            cout << "Success serialize to string: " << tmpBuffer << endl;
        }

        my::helloworld msg2;
        std::string tmpStr(tmpBuffer);
        if (!msg2.ParseFromString(tmpStr)) {
            cout << "Failed to parse from string." << endl;
            return -1;
        } else {
            cout << "Success parse from string." << endl;
            ListMsg(msg2);
        }

        my::helloworld msg3;
        if (!msg3.ParseFromArray(tmpBuffer, strlen(tmpBuffer))) {
            cout << "Failed to parse from array." << endl;
            return -1;
        } else {
            cout << "Success parse from array." << endl;
            ListMsg(msg3);
        }
    }

    // pd 结构体转 C 数组
    // SerializeToArray
    // ParseFromArray

    return 0;
}