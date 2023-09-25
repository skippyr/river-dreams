#include "Shell.hpp"

using namespace RiverDreams::InputOutput;

std::string Shell::GetTotalOfBackgroundJobs()
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
