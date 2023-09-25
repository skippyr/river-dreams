#pragma once

#include <string>

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

namespace RiverDreams::Connectivity
{
    class Network
    {
    private:
        static bool        IsValidIPV4NetworkInterface(struct ifaddrs* networkInterface);
        static std::string GetNetworkInterfaceIPV4Address(struct ifaddrs* networkInterface);

    public:
        static std::string GetLocalIPV4Address();
        static std::string GetHostName();
    };
}
