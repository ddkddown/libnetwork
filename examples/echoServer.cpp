#include "ServerFac.h"
#include "PollWrapper.h"
#include "Logger.h"

int main() {
    ServerFac factory;
    auto server = factory.CreateEchoServer(9677, 10);
    // make_shared<EchoServer>(9677, 10);
    Logger::InitLog("./test.log");
    LOG_INFO<<"start";

    PollWrapper poller;
    poller.addEvent(server);
    poller.Run();

    LOG_INFO<<"end";
}