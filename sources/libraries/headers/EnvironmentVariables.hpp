#pragma once

#include <string>

namespace RiverDreams
{
    class EnvironmentVariables
    {
    private:
        static std::string GetEnvironmentVariable(std::string environmentVariable);

    public:
        static std::string GetPWD();
        static std::string GetVirtualEnvironment();
    };
}
