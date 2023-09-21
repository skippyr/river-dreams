#pragma once

#include <string>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

class Network
{
public:
    static std::string GetLocalIPV4Address();
    static std::string GetHostName();
};
