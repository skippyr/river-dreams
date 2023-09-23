#pragma once

#include <string>

#include <sys/statvfs.h>

namespace RiverDreams
{
    class StorageDevice
    {
    public:
        static unsigned short GetUsagePercentage();
    };
}
