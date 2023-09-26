#include "StorageDevice.hpp"

#define FILE_SYSTEM_ROOT_MOUNT_POINT "/"

using namespace RiverDreams::FileSystem;

unsigned short StorageDevice::GetUsagePercentage()
{
    struct statvfs storageDeviceProperties;
    if (statvfs(FILE_SYSTEM_ROOT_MOUNT_POINT, &storageDeviceProperties))
    {
        return 0;
    }
    unsigned long totalSizeInBytes =
        storageDeviceProperties.f_blocks * storageDeviceProperties.f_frsize;
    unsigned long freeSizeInBytes =
        storageDeviceProperties.f_bfree * storageDeviceProperties.f_frsize;
    unsigned long usedSizeInBytes = totalSizeInBytes - freeSizeInBytes;
    return ((float)usedSizeInBytes / totalSizeInBytes) * 100;
}
