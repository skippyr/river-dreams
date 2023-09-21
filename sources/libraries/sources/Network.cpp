#include "Network.hpp"

std::string Network::GetLocalIPV4Address()
{
    std::string     loopbackAddress = "127.0.0.1";
    struct ifaddrs* localNetworkInterfaces;
    if (getifaddrs(&localNetworkInterfaces))
    {
        return (loopbackAddress);
    }
    for (struct ifaddrs* localNetworkInterface = localNetworkInterfaces; localNetworkInterface;
         localNetworkInterface                 = localNetworkInterface->ifa_next)
    {
        if (!localNetworkInterface->ifa_addr || !(localNetworkInterface->ifa_flags & IFF_RUNNING) ||
            localNetworkInterface->ifa_flags & IFF_LOOPBACK || localNetworkInterface->ifa_addr->sa_family != AF_INET)
        {
            continue;
        }
        char localAddress[16];
        if (!inet_ntop(localNetworkInterface->ifa_addr->sa_family,
                       &((struct sockaddr_in*)localNetworkInterface->ifa_addr)->sin_addr, localAddress,
                       sizeof(localAddress)))
        {
            continue;
        }
        freeifaddrs(localNetworkInterfaces);
        return (localAddress);
    }
    freeifaddrs(localNetworkInterfaces);
    return (loopbackAddress);
}

std::string Network::GetHostName()
{
    return ("%m");
}
