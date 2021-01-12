#pragma once
#include "VServerFac.h"

class ServerFac : public VServerFac {
public:
    SharedServer CreateEchoServer(int port, int lSize);
    SharedServer CreateEpollServer(int port, int lSize,
                        const std::shared_ptr<PollWrapper> &poller);
};