#pragma once
#include <iostream>
#include "EchoServer.h"
#include "TcpServer.h"

using namespace std;

class VServerFac {
public:
    virtual SharedServer CreateEchoServer(int port, int lSize) = 0;
};