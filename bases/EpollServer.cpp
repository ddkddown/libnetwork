#include "EpollServer.h"

void EpollServer::ReadHandle() {
    auto client = make_shared<TcpClient>(Accept());
    poller_->addEvent(client);
}

void EpollServer::WriteHandle() {

}