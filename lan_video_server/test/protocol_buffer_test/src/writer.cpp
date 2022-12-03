// writer.cpp
#include <iostream>
#include <string>
#include <fstream>
#include "my.pb.h"

using namespace std;

int main()
{
    string str = "hello";
    my::helloworld msg1;
    msg1.set_id(101);
    msg1.set_str(str);
    // Write the new address book back to disk.
    fstream output("hhh.txt", ios::out | ios::trunc | ios::binary);
    cout << "hhh" << endl;
    // SerializeToOstream 序列化到 Ostream
    if (!msg1.SerializeToOstream(&output))
    {
        cout << "Failed to write msg." << endl;
        return -1;
    }
    return 0;
}