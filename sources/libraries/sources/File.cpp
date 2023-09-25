#include "File.hpp"

using namespace RiverDreams::FileSystem;

File::File(std::string path)
{
    std::string readMode = "r";
    stream               = std::fopen(path.c_str(), readMode.c_str());
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
