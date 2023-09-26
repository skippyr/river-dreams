#include "Network.hpp"

#define IPV4_ADDRESS_MAXIMUM_LENGTH 16
#define IPV4_FAMILY_IDENTIFIER      AF_INET
#define LOOPBACK_IPV4_ADDRESS       "127.0.0.1"

using namespace RiverDreams::Connectivity;

bool Network::IsValidIPV4NetworkAddress(struct ifaddrs* networkInterface)
{
    bool hasIPV4Address = networkInterface->ifa_addr &&
                          networkInterface->ifa_addr->sa_family == IPV4_FAMILY_IDENTIFIER;
    bool isRunning      = networkInterface->ifa_flags & IFF_RUNNING;
    bool isLoopback     = networkInterface->ifa_flags & IFF_LOOPBACK;
    return hasIPV4Address && isRunning && !isLoopback;
}

std::string Network::GetNetworkInterfaceIPV4Address(struct ifaddrs* networkInterface)
{
    char address[IPV4_ADDRESS_MAXIMUM_LENGTH];
    return IsValidIPV4NetworkAddress(networkInterface) &&
           inet_ntop(IPV4_FAMILY_IDENTIFIER,
                     &((struct sockaddr_in*)networkInterface->ifa_addr)->sin_addr, address,
                     sizeof(address))
               ? address
               : "";
}

std::string Network::GetLocalIPV4Address()
{
    struct ifaddrs* localNetworkInterfaces;
    if (getifaddrs(&localNetworkInterfaces))
    {
        return LOOPBACK_IPV4_ADDRESS;
    }
    for (struct ifaddrs* localNetworkInterface = localNetworkInterfaces; localNetworkInterface;
         localNetworkInterface = localNetworkInterface->ifa_next)
    {
        std::string address = GetNetworkInterfaceIPV4Address(localNetworkInterface);
        if (address != "")
        {
            freeifaddrs(localNetworkInterfaces);
            return address;
        }
    }
    freeifaddrs(localNetworkInterfaces);
    return LOOPBACK_IPV4_ADDRESS;
}

std::string Network::GetHostName()
{
    return "%m";
}
