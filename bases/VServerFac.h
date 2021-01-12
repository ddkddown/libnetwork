#pragma once
#include <iostream>
#include "EchoServer.h"
#include "TcpServer.h"
#include "EpollServer.h"
#include "PollWrapper.h"

using namespace std;

class VServerFac {
public:
    virtual SharedServer CreateEchoServer(int port, int lSize) = 0;

    virtual SharedServer CreateEpollServer(int port, int lSize,
                        const std::shared_ptr<PollWrapper> &poller) = 0;
};