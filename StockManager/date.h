

#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <string>
using namespace std;

class Date
{

public:

    // Formats for asString
    enum EDateFormats {
        EDateFormatEpoch,
        EDateFormatYYMMDD,
        EDateFormatYYYYMMMDD
    };

    // Constructor: Create object with passed date as 3 variables
    Date(const unsigned int year, const int month, const int day);

    // Constructor: Create object with passed date as epoch
    Date(const time_t epoch);

    // Constructor: Create an invalid date
    Date();

    // Prints the contained date as a string, in the format specified by 'format'
    string asString(const EDateFormats format) const;

    // Returns this object's day
    unsigned int day() const;

    // Returns this object's month
    unsigned int Month() const;

    // Returns this object's year
    unsigned int Year() const;

    // If this object's date is greater than the comparisonDate, then return true. Otherwise return false.
    bool operator>(const Date comparisonDate) const;

    // If this object's date is less than the comparisonDate, then return true. Otherwise return false.
    bool operator<(const Date comparisonDate) const;

    // create and return a new date object which has this object's date plus 'days' number of days.
    Date operator+(const int days) const;

    // create and return a new date object which has this object's date minus 'days' number of days.
    Date operator-(const int days) const;

    // subtract comparisonDate's day from this object's day, and return the result.
    int operator-(const Date comparisonDate) const;

    // If this object and comparisonDate are the exact same date, then return true. Otherwise return false.
    bool operator==(const Date comparisonDate) const;

    // If this object and comparisonDate are different dates, then return true. Otherwise return false.
    bool operator!=(const Date comparisonDate) const;

    // True if this is a valid date (i.e. not 0/0/0)
    bool isValid() const;

private:
    // day of the month.
    unsigned int m_day;

    // month of the year.
    unsigned int m_month;

    // year of the gregorian calendar.
    unsigned int m_year;

    // Returns epoch
    time_t asEpoch() const;

};


#endif // DATE_H
