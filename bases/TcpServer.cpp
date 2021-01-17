#include "TcpServer.h"

int TcpServer::Accept() {
    auto tmp = sock_.Accept();
    LOG_DEBUG<<"get client: "<<tmp;
    return tmp;
}