#pragma once

namespace RiverDreams::FileSystem
{
    struct DirectoryEntriesStatus
    {
    public:
        unsigned long totalOfHiddenEntries     = 0;
        unsigned long totalOfExecutableEntries = 0;
        unsigned long totalOfSymlinkEntries    = 0;
    };
}
