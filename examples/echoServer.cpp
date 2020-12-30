#include "Header.h"

network::SocketWrapper sock(9677);

void ReadHandle() {
    char buf[1024] = {0};
    auto peer = sock.Accept();
    read(peer, buf, 1024);
    write(peer, buf, 1024);
}
int main() {
    Logger::InitLog("./test.log");
    LOG_INFO<<"start";
    
    sock.Bind();
    sock.Listen(10);

    FdWrapper fd(sock.GetSockFd(), READ, ReadHandle);
    PollWrapper poller;
    poller.addEvent(fd);
    poller.Run();

    LOG_INFO<<"end";
}