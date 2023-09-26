#include "File.hpp"

#define READ_MODE "r"

using namespace RiverDreams::FileSystem;

File::File(std::string path)
{
    stream = std::fopen(path.c_str(), READ_MODE);
}

File::~File()
{
    if (stream)
    {
        std::fclose(stream);
    }
}

char File::GetCharacter()
{
    return stream ? std::fgetc(stream) : EOF;
}
