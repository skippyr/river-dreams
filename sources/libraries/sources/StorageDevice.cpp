#include "StorageDevice.hpp"

#define SYSTEM_MOUNT_POINT "/"

using namespace RiverDreams::FileSystem;

unsigned short StorageDevice::GetUsagePercentage()
{
    struct statvfs deviceProperties;
    if (statvfs(SYSTEM_MOUNT_POINT, &deviceProperties))
    { return 0; }
    unsigned long totalSize = deviceProperties.f_blocks * deviceProperties.f_frsize;
    unsigned long freeSize  = deviceProperties.f_bfree * deviceProperties.f_frsize;
    unsigned long usedSize  = totalSize - freeSize;
    return ((float)usedSize / totalSize) * 100;
}
