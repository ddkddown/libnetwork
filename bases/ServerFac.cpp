#include "ServerFac.h"

SharedServer ServerFac::CreateEchoServer(int port, int lSize) {
    SharedServer tmp =  make_shared<EchoServer>(port, lSize);
    return tmp;
}

SharedServer ServerFac::CreateEpollServer(int port, int lSize,
                        const std::shared_ptr<PollWrapper> &poller) {
    SharedServer tmp = make_shared<EpollServer>(port, lSize, poller);
    return tmp;
}