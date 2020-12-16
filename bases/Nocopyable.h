#pragma once
#include <iostream>

namespace network{
class Nocopy
{
    private:
        Nocopy(const Nocopy &v) = delete;
        Nocopy& operator=(const Nocopy &v) = delete;
        Nocopy(const Nocopy &&v) = delete;
        Nocopy& operator=(const Nocopy &&v) = delete;
    protected:
        Nocopy() = default;
        ~Nocopy() = default;
};
}