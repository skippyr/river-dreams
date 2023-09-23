#pragma once

#include <string>

namespace RiverDreams
{
    class Shell
    {
    public:
        static std::string GetQuantityOfBackgroundJobs();
        static std::string WrapOnBackgroundJobEvent(std::string trueCaseText, std::string falseCaseText);
        static std::string WrapOnErrorEvent(std::string trueCaseText, std::string falseCaseText);
    };
}
