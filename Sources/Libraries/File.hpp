#pragma once

#include <cstdio>
#include <string>

namespace RiverDreams::FileSystem
{
    class File
    {
    private:
        std::FILE* stream;

    public:
        File(std::string path);
        ~File();
        char GetCharacter();
    };
}
