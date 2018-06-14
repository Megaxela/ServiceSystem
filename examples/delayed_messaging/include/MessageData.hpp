#pragma once

#include <chrono>
#include <string>

struct MessageData
{
    std::string string;
    std::chrono::milliseconds ms;
};