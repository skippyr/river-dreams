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
        struct std::tm properties;

        std::string GetWeekDay();
        std::string GetMonth();
        std::string GetDayOrdinal();

    public:
        SystemTime();
        std::string GetCalendar();
        std::string GetClock();
        DayMoment   GetDayMoment();
    };
}
