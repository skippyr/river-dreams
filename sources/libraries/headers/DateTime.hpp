#pragma once

#include "DayMoment.hpp"

#include <cstring>
#include <ctime>
#include <string>

namespace RiverDreams::Time
{
    class DateTime
    {
    private:
        struct std::tm localTime;

        std::string GetWeekDay();
        std::string GetMonth();
        std::string GetDayOrdinal();

    public:
        DateTime();
        std::string GetCalendar();
        std::string GetClock();
        DayMoment   GetDayMoment();
    };
}
