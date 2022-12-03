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
    }

    ListMsg(msg1);
    return 0;
}