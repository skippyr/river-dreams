#include "Network.hpp"

#define LOOPBACK_NETWORK_INTERFACE_IPV4_ADDRESS "127.0.0.1"
#define IPV4_FAMILY                             AF_INET
#define MAXIMUM_IPV4_ADDRESS_LENGTH             16

using namespace RiverDreams::Connectivity;

bool Network::IsValidIPV4NetworkInterface(struct ifaddrs* networkInterface)
{
    bool hasIPV4Address =
        networkInterface->ifa_addr && networkInterface->ifa_addr->sa_family == IPV4_FAMILY;
    bool isRunning  = networkInterface->ifa_flags & IFF_RUNNING;
    bool isLoopback = networkInterface->ifa_flags & IFF_LOOPBACK;
    return hasIPV4Address && isRunning && !isLoopback;
}

std::string Network::GetNetworkInterfaceIPV4Address(struct ifaddrs* networkInterface)
{
    char networkInterfaceIPV4Address[MAXIMUM_IPV4_ADDRESS_LENGTH];
    return IsValidIPV4NetworkInterface(networkInterface) &&
                   inet_ntop(IPV4_FAMILY,
                             &((struct sockaddr_in*)networkInterface->ifa_addr)->sin_addr,
                             networkInterfaceIPV4Address, sizeof(networkInterfaceIPV4Address))
               ? networkInterfaceIPV4Address
               : "";
}

std::string Network::GetLocalIPV4Address()
{
    struct ifaddrs* localNetworkInterfaces;
    if (getifaddrs(&localNetworkInterfaces))
    {
        return LOOPBACK_NETWORK_INTERFACE_IPV4_ADDRESS;
    }
    for (struct ifaddrs* localNetworkInterface = localNetworkInterfaces; localNetworkInterface;
         localNetworkInterface                 = localNetworkInterface->ifa_next)
    {
        std::string localNetworkInterfaceIPV4Address =
            GetNetworkInterfaceIPV4Address(localNetworkInterface);
        if (localNetworkInterfaceIPV4Address != "")
        {
            freeifaddrs(localNetworkInterfaces);
            return localNetworkInterfaceIPV4Address;
        }
    }
    freeifaddrs(localNetworkInterfaces);
    return LOOPBACK_NETWORK_INTERFACE_IPV4_ADDRESS;
}

std::string Network::GetHostName()
{
    return "%m";
}
