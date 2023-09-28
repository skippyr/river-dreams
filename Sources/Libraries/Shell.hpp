#pragma once

#include <string>

namespace RiverDreams::IO
{
    class Shell
    {
    public:
        static std::string GetTotalOfBackgroundJobs();
        static std::string WrapOnErrorEvent(std::string trueCaseText  = "",
                                            std::string falseCaseText = "");
        static std::string WrapOnBackgroundJobEvent(std::string trueCaseText  = "",
                                                    std::string falseCaseText = "");
    };
}
