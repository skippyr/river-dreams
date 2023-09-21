#pragma once

#include <string>

class EnvironmentVariables
{
private:
    static std::string GetEnvironmentVariable(std::string environment_variable);

public:
    static std::string GetPWD();
    static std::string GetVirtualEnvironment();
};
