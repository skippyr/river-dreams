#include "StorageDevice.hpp"

unsigned short StorageDevice::GetUsagePercentage()
{
    struct statvfs storageDeviceProperties;
    std::string systemMountPoint = "/";
    if (statvfs(systemMountPoint.c_str(), &storageDeviceProperties))
    {
        return 0;
    }
    unsigned long totalSizeInBytes = storageDeviceProperties.f_blocks * storageDeviceProperties.f_frsize;
    unsigned long freeSizeInBytes = storageDeviceProperties.f_bfree * storageDeviceProperties.f_frsize;
    unsigned long usedSizeInBytes = totalSizeInBytes - freeSizeInBytes;
    return ((float)usedSizeInBytes / totalSizeInBytes) * 100;
}
