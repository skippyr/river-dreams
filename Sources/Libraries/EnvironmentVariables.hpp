#pragma once

#include <string>

namespace RiverDreams::Environment
{
    class EnvironmentVariables
    {
    private:
        static std::string GetVariable(std::string variable);

    public:
        static std::string GetPWD();
        static std::string GetVirtualEnv();
    };
}
