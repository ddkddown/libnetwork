#include "ServerFac.h"
#include "PollWrapper.h"
#include "Logger.h"

int main() {
    auto poller = make_shared<PollWrapper>();
    ServerFac factory;
    auto server = factory.CreateEpollServer(9677, 10, poller);
    Logger::InitLog("./test.log");
    LOG_INFO<<"start";

    
    poller->addEvent(server);
    poller->Run();

    LOG_INFO<<"end";
}