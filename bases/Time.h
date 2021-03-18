#pragma once
#include <chrono>

#define HEART_BEAT_TIME 10.0
using TIMESTAMP = chrono::time_point<chrono::system_clock>;
using ELAPSED = chrono::duration<double>;
