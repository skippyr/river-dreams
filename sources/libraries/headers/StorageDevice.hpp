#pragma once

#include <string>

#include <sys/statvfs.h>

class StorageDevice
{
public:
    static unsigned short GetUsagePercentage();
};
