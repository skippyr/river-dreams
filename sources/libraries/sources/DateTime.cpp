#include "DateTime.hpp"

#define UNDEFINED_TIME "---"

using namespace RiverDreams::Time;

std::string DateTime::GetWeekDay()
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
    }
    return UNDEFINED_TIME;
}

std::string DateTime::GetMonth()
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
    }
    return UNDEFINED_TIME;
}

std::string DateTime::GetDayOrdinal()
{
    bool isFirst  = !((localTime.tm_mday - 1) % 10);
    bool isSecond = !((localTime.tm_mday - 2) % 10);
    bool isThird  = !((localTime.tm_mday - 3) % 10);
    if (isFirst)
    { return "st"; }
    else if (isSecond)
    { return "nd"; }
    else if (isThird)
    { return "rd"; }
    return "th";
}

DateTime::DateTime()
{
    std::time_t epochTime = std::time(0);
    if (!localtime_r(&epochTime, &localTime))
    { std::memset(&localTime, 0, sizeof(localTime)); }
}

std::string DateTime::GetCalendar()
{
    return "(" + GetWeekDay() + ") " + GetMonth() + " " + std::to_string(localTime.tm_mday)
               + GetDayOrdinal();
}

std::string DateTime::GetClock()
{
    std::string hourPadding   = localTime.tm_hour < 10 ? "0" : "";
    std::string minutePadding = localTime.tm_min < 10 ? "0" : "";
    std::string hourUnit      = "h";
    std::string minuteUnit    = "m";
    return hourPadding + std::to_string(localTime.tm_hour) + hourUnit + minutePadding
                       + std::to_string(localTime.tm_min) + minuteUnit;
}

DayMoment DateTime::GetDayMoment()
{
    bool isDawn      = localTime.tm_hour < 6;
    bool isMorning   = !isDawn && localTime.tm_hour < 12;
    bool isAfternoon = !isMorning && localTime.tm_hour < 18;
    if (isDawn)
    { return DayMoment::Dawn; }
    else if (isMorning)
    { return DayMoment::Morning; }
    else if (isAfternoon)
    { return DayMoment::Afternoon; }
    return DayMoment::Night;
}
