#include "SystemTime.hpp"

using namespace RiverDreams;

std::string SystemTime::GetCalendarWeekDayAbbreviated()
{
    switch (this->localTime->tm_wday)
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
        return "---";
    }
}

std::string SystemTime::GetCalendarMonthAbbreviated()
{
    switch (this->localTime->tm_mon)
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
        return "---";
    }
}

std::string SystemTime::GetCalendarOrdinal()
{
    bool isFirstOrdinal  = !((this->localTime->tm_mday - 1) % 10);
    bool isSecondOrdinal = !((this->localTime->tm_mday - 2) % 10);
    bool isThirdOrdinal  = !((this->localTime->tm_mday - 3) % 10);
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
    std::time_t epochTime = std::time(0);
    this->localTime       = std::localtime(&epochTime);
}

std::string SystemTime::GetCalendar()
{
    std::string weekDayAbbeviated = GetCalendarWeekDayAbbreviated();
    std::string monthAbbreviated  = GetCalendarMonthAbbreviated();
    std::string ordinal           = GetCalendarOrdinal();
    std::string monthDay          = std::to_string(this->localTime->tm_mday);
    return "(" + weekDayAbbeviated + ") " + monthAbbreviated + " " + monthDay + ordinal;
}

std::string SystemTime::GetClock()
{
    std::string hoursLeftPadding   = this->localTime->tm_hour < 10 ? "0" : "";
    std::string minutesLeftPadding = this->localTime->tm_min < 10 ? "0" : "";
    std::string hours              = std::to_string(this->localTime->tm_hour);
    std::string minutes            = std::to_string(this->localTime->tm_min);
    std::string hourUnit           = "h";
    std::string minuteUnit         = "m";
    return hoursLeftPadding + hours + hourUnit + minutesLeftPadding + minutes + minuteUnit;
}

std::string SystemTime::GetColoredClockSymbol()
{
    bool isDawn      = this->localTime->tm_hour < 6;
    bool isMorning   = this->localTime->tm_hour < 12;
    bool isAfternoon = this->localTime->tm_hour < 18;
    if (isDawn)
    {
        return Terminal::ApplyForegroundColor(Color::Magenta, "󰭎 ");
    }
    else if (isMorning)
    {
        return Terminal::ApplyForegroundColor(Color::Red, "󰖨 ");
    }
    else if (isAfternoon)
    {
        return Terminal::ApplyForegroundColor(Color::Blue, " ");
    }
    return Terminal::ApplyForegroundColor(Color::Yellow, "󰽥 ");
}
