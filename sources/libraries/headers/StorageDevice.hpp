#pragma once

#include <string>

#include <sys/statvfs.h>

namespace RiverDreams::FileSystem
{
    class StorageDevice
    {
    public:
        static unsigned short GetUsagePercentage();
    };
}
