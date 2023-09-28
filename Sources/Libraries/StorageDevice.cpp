#include "StorageDevice.hpp"

#define SYSTEM_MOUNT_POINT "/"

using namespace RiverDreams::FileSystem;

unsigned short StorageDevice::GetUsagePercentage()
{
    struct statvfs properties;
    if (statvfs(SYSTEM_MOUNT_POINT, &properties))
    { return (0); }
    unsigned long totalSize = properties.f_blocks * properties.f_frsize;
    unsigned long freeSize  = properties.f_bfree * properties.f_frsize;
    unsigned long usedSize  = totalSize - freeSize;
    return (((float)usedSize / totalSize) * 100);
}
