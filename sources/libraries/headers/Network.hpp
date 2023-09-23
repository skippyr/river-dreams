#pragma once

#include <string>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

namespace RiverDreams
{
    class Network
    {
    public:
        static std::string GetLocalIPV4Address();
        static std::string GetHostName();
    };
}
