#pragma once
#include "TcpClient.h"
#include "TcpServer.h"
#include "EchoClient.h"
#include "PollWrapper.h"

class EpollServer : public TcpServer{
public:
    EpollServer(int port, int lSize,
                const std::shared_ptr<PollWrapper> &poller)
                :TcpServer(port, lSize), poller_(poller) {}
    void ReadHandle();
    void WriteHandle();
private:
    std::shared_ptr<PollWrapper> poller_;
};