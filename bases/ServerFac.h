#pragma once
#include "VServerFac.h"

class ServerFac : public VServerFac {
public:
    SharedServer CreateEchoServer(int port, int lSize);
};