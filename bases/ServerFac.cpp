#include "ServerFac.h"

SharedServer ServerFac::CreateEchoServer(int port, int lSize) {
    SharedServer tmp =  make_shared<EchoServer>(port, lSize);
    return tmp;
}