#include "Time.h"


int MyTime::Week = 4;
int MyTime::Month = 11;
int MyTime::Day = 30;
int MyTime::Year = 2024;
MyTime* MyTime::instance = nullptr;

int getYear(string date) {
    // Assuming the date format is "DD-MM-YYYY"
    return stoi(date.substr(date.size() - 4));
}
int getWeekNumberInMonth(const std::string& dateStr) {
    // Parse the input string in dd-mm-yyyy format
    int day, month, year;
    char delimiter1, delimiter2;
    std::istringstream dateStream(dateStr);
    dateStream >> day >> delimiter1 >> month >> delimiter2 >> year;

    if (delimiter1 != '-' || delimiter2 != '-' || dateStream.fail()) {
        throw std::invalid_argument("Invalid date format. Expected format: dd-mm-yyyy");
    }

    // Create a tm struct for the first day of the given month
    std::tm firstDayOfMonth = {};
    firstDayOfMonth.tm_mday = 1;
    firstDayOfMonth.tm_mon = month - 1; // tm_mon is 0-based
    firstDayOfMonth.tm_year = year - 1900; // tm_year is years since 1900

    // Normalize the first day of the month
    std::mktime(&firstDayOfMonth);

    // Get the weekday for the 1st day of the month (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    int firstDayWeekday = (firstDayOfMonth.tm_wday == 0) ? 7 : firstDayOfMonth.tm_wday; // Adjust so Monday = 1, ..., Sunday = 7

    // Calculate the week number
    int adjustedDay = day + (firstDayWeekday - 1); // Account for days before the 1st of the month
    int weekNumber = (adjustedDay - 1) / 7 + 1;

    return weekNumber;
}
int getDayInWeek(int dayOfYear) {
    // Days are counted starting from 1 (January 1st)
    int dayOfWeek = (dayOfYear - 1) % 7 + 1; // Calculate the position within the week
    return dayOfWeek;
}

int getDayGivenMonth(const std::string& dateStr) {
    // Parse the input string in dd-mm-yyyy format
    int day, month, year;
    char delimiter1, delimiter2;
    std::istringstream dateStream(dateStr);
    dateStream >> day >> delimiter1 >> month >> delimiter2 >> year;

    if (delimiter1 != '-' || delimiter2 != '-' || dateStream.fail()) {
        throw std::invalid_argument("Invalid date format. Expected format: dd-mm-yyyy");
    }

    // Create a tm struct for the first day of the given month
    std::tm firstDayOfMonth = {};
    firstDayOfMonth.tm_mday = 1;
    firstDayOfMonth.tm_mon = month - 1; // tm_mon is 0-based
    firstDayOfMonth.tm_year = year - 1900; // tm_year is years since 1900

    // Normalize the first day of the month
    std::mktime(&firstDayOfMonth);

    // Get the weekday for the 1st day of the month (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    int firstDayWeekday = (firstDayOfMonth.tm_wday == 0) ? 7 : firstDayOfMonth.tm_wday; // Adjust so Monday = 1, ..., Sunday = 7

    // Calculate the day in the week for the given day
    int offset = (day + (firstDayWeekday - 2)) % 7; // Offset to calculate the exact day in the week
    int dayInWeek = (offset == 0) ? 7 : offset;     // Adjust for Sunday as the last day of the week

    return dayInWeek;
}
