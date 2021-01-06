#include "EchoServer.h"

void EchoServer::ReadHandle() {
    auto client = make_shared<EchoClient>(Accept());
    char buff[1024] = {0};
    client->Read(1024, buff);
    client->Write(1024, buff);
}

void EchoServer::WriteHandle() {}