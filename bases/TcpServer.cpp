#include "TcpServer.h"

int TcpServer::Accept() {
    return sock_.Accept();
}