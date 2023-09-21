#include "EnvironmentVariables.hpp"

std::string EnvironmentVariables::GetEnvironmentVariable(std::string environmentVariable)
{
    const char* value = std::getenv(environmentVariable.c_str());
    return (value ? value : "");
}

std::string EnvironmentVariables::GetPWD()
{
    return (GetEnvironmentVariable("PWD"));
}

std::string EnvironmentVariables::GetVirtualEnvironment()
{
    return (GetEnvironmentVariable("VIRTUAL_ENV"));
}
