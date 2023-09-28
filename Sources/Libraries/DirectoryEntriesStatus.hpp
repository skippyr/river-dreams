#pragma once

namespace RiverDreams::FileSystem
{
    struct DirectoryEntriesStatus
    {
        unsigned long totalOfHiddenEntries     = 0;
        unsigned long totalOfSymlinkEntries    = 0;
        unsigned long totalOfExecutableEntries = 0;
    };
}
