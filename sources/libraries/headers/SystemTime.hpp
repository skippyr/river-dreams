#pragma once

#include "DayMoment.hpp"

#include <cstring>
#include <ctime>
#include <string>

namespace RiverDreams::Time
{
    class SystemTime
    {
    private:
        struct std::tm localTime;

    private:
        std::string GetMonthAbbreviated();
        std::string GetWeekDayAbbreviated();
        std::string GetDayOrdinal();

    public:
        SystemTime();
        std::string GetCalendar();
        std::string GetClock();
        DayMoment   GetDayMoment();
    };
}
