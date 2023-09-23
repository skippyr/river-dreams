#include "Shell.hpp"

std::string Shell::GetQuantityOfBackgroundJobs()
{
    return "%j";
}

std::string Shell::WrapOnBackgroundJobEvent(std::string trueCaseText, std::string falseCaseText)
{
    return "%(1j." + trueCaseText + "." + falseCaseText + ")";
}

std::string Shell::WrapOnErrorEvent(std::string trueCaseText, std::string falseCaseText)
{
    return "%(?." + falseCaseText + "." + trueCaseText + ")";
}
