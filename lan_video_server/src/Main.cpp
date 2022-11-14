#include "Tcp_Server.h"
#include "LanVideoServer.h"
#include "LanDefine.h"

// Tcp_Sever +port

void User(char *name)
{
    std::cout << name << " + port" << std::endl;
}

int main(int argc, char *argv[])
{
    LanVideoServer lan_server(LAN_CONFIG_PATH);
    lan_server.Init();
    lan_server.Run();
    return 0;
}
