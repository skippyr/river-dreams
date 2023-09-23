#pragma once

#include "Terminal.hpp"

#include <ctime>

namespace RiverDreams
{
    class SystemTime
    {
    private:
        struct std::tm* localTime;

        std::string GetCalendarWeekDayAbbreviated();
        std::string GetCalendarMonthAbbreviated();
        std::string GetCalendarOrdinal();

    public:
        SystemTime();
        std::string GetCalendar();
        std::string GetClock();
        std::string GetColoredClockSymbol();
    };
}
