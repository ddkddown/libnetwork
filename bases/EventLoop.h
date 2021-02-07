#pragma once;
#include <iostream>
#include "Channel.h"

class EventLoop {
public:
    void UpdateChannel(Channel &c, int fd);
};