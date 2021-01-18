#include "EpollServer.h"

void EpollServer::ReadHandle() {
    auto client = make_shared<TcpClient>(Accept());
    if (-1 == client->GetFd()) {
        LOG_ERR<<"get client fd is -1, err happen:"<<errno;
        return;
    }
    
    poller_->addEvent(client);
}

void EpollServer::WriteHandle() {

}