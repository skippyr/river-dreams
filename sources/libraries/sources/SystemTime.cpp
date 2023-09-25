#include "SystemTime.hpp"

#define UNDEFINED_TIME "---"

using namespace RiverDreams::Time;

std::string SystemTime::GetMonthAbbreviated()
{
    switch (localTime.tm_mon)
    {
    case 0:
        return "Jan";
    case 1:
        return "Feb";
    case 2:
        return "Mar";
    case 3:
        return "Apr";
    case 4:
        return "May";
    case 5:
        return "Jun";
    case 6:
        return "Jul";
    case 7:
        return "Aug";
    case 8:
        return "Sep";
    case 9:
        return "Oct";
    case 10:
        return "Nov";
    case 11:
        return "Dec";
    default:
        return UNDEFINED_TIME;
    }
}

std::string SystemTime::GetWeekDayAbbreviated()
{
    switch (localTime.tm_wday)
    {
    case 0:
        return "Sun";
    case 1:
        return "Mon";
    case 2:
        return "Tue";
    case 3:
        return "Wed";
    case 4:
        return "Thu";
    case 5:
        return "Fri";
    case 6:
        return "Sat";
    default:
        return UNDEFINED_TIME;
    }
}

std::string SystemTime::GetDayOrdinal()
{
    bool isFirstOrdinal  = !((localTime.tm_mday - 1) % 10);
    bool isSecondOrdinal = !((localTime.tm_mday - 2) % 10);
    bool isThirdOrdinal  = !((localTime.tm_mday - 3) % 10);
    if (isFirstOrdinal)
    {
        return "st";
    }
    else if (isSecondOrdinal)
    {
        return "nd";
    }
    else if (isThirdOrdinal)
    {
        return "rd";
    }
    return "th";
}

SystemTime::SystemTime()
{
    long epochTime = std::time(0);
    if (!localtime_r(&epochTime, &localTime))
    {
        std::memset(&localTime, 0, sizeof(localTime));
    }
}

std::string SystemTime::GetCalendar()
{
    return "(" + GetWeekDayAbbreviated() + ") " + GetMonthAbbreviated() + " " + std::to_string(localTime.tm_mday) +
           GetDayOrdinal();
}

std::string SystemTime::GetClock()
{
    std::string hoursLeftPadding   = localTime.tm_hour < 10 ? "0" : "";
    std::string minutesLeftPadding = localTime.tm_min < 10 ? "0" : "";
    std::string hourUnit           = "h";
    std::string minuteUnit         = "m";
    return hoursLeftPadding + std::to_string(localTime.tm_hour) + hourUnit + minutesLeftPadding +
           std::to_string(localTime.tm_min) + minuteUnit;
}

DayMoment SystemTime::GetDayMoment()
{
    bool isDawn      = localTime.tm_hour < 6;
    bool isMorning   = !isDawn && localTime.tm_hour < 12;
    bool isAfternoon = !isMorning && localTime.tm_hour < 18;
    if (isDawn)
    {
        return DayMoment::Dawn;
    }
    else if (isMorning)
    {
        return DayMoment::Morning;
    }
    else if (isAfternoon)
    {
        return DayMoment::Afternoon;
    }
    return DayMoment::Night;
}
