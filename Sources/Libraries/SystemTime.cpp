#include "SystemTime.hpp"

#define UNDEFINED_TIME "---"

using namespace RiverDreams::Time;

std::string SystemTime::GetWeekDay()
{
    switch (properties.tm_wday)
    {
    case (0):
        return ("Sun");
    case (1):
        return ("Mon");
    case (2):
        return ("Tue");
    case (3):
        return ("Wed");
    case (4):
        return ("Thu");
    case (5):
        return ("Fri");
    case (6):
        return ("Sat");
    }
    return (UNDEFINED_TIME);
}

std::string SystemTime::GetMonth()
{
    switch (properties.tm_mon)
    {
    case (0):
        return ("Jan");
    case (1):
        return ("Feb");
    case (2):
        return ("Mar");
    case (3):
        return ("Apr");
    case (4):
        return ("May");
    case (5):
        return ("Jun");
    case (6):
        return ("Jul");
    case (7):
        return ("Aug");
    case (8):
        return ("Sep");
    case (9):
        return ("Oct");
    case (10):
        return ("Nov");
    case (11):
        return ("Dec");
    }
    return (UNDEFINED_TIME);
}

std::string SystemTime::GetDayOrdinal()
{
    bool isFirst  = !((properties.tm_mday - 1) % 10);
    bool isSecond = !((properties.tm_mday - 2) % 10);
    bool isThird  = !((properties.tm_mday - 3) % 10);
    if (isFirst)
    { return ("st"); }
    else if (isSecond)
    { return ("nd"); }
    else if (isThird)
    { return ("rd"); }
    return ("th");
}

SystemTime::SystemTime()
{
    std::time_t epochTime = std::time(0);
    if (!localtime_r(&epochTime, &properties))
    { std::memset(&properties, 0, sizeof(properties)); }
}

std::string SystemTime::GetCalendar()
{
    return ("(" + GetWeekDay() + ") " + GetMonth() + " " + std::to_string(properties.tm_mday)
                + GetDayOrdinal());
}

std::string SystemTime::GetClock()
{
    std::string hourPadding   = properties.tm_hour < 10 ? "0" : "";
    std::string minutePadding = properties.tm_min < 10 ? "0" : "";
    std::string hourUnit      = "h";
    std::string minuteUnit    = "m";
    return (hourPadding + std::to_string(properties.tm_hour) + hourUnit + minutePadding
                        + std::to_string(properties.tm_min) + minuteUnit);
}

DayMoment SystemTime::GetDayMoment()
{
    bool isDawn      = properties.tm_hour < 6;
    bool isMorning   = !isDawn && properties.tm_hour < 12;
    bool isAfternoon = !isMorning && properties.tm_hour < 18;
    if (isDawn)
    { return (DayMoment::Dawn); }
    else if (isMorning)
    { return (DayMoment::Morning); }
    else if (isAfternoon)
    { return (DayMoment::Afternoon); }
    return (DayMoment::Night);
}
