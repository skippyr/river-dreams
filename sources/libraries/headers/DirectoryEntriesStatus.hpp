#pragma once

namespace RiverDreams::FileSystem
{
    class DirectoryEntriesStatus
    {
    public:
        unsigned long totalOfHiddenEntries     = 0;
        unsigned long totalOfSymlinkEntries    = 0;
        unsigned long totalOfExecutableEntries = 0;
    };
}
