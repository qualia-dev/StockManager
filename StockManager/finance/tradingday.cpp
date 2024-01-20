

#include "tradingday.h"


// Set the starting values for this TradingDay object
TradingDay::TradingDay(
        const QDate& date,
        const float time,
        const float open,
        const float close,
        const float high,
        const float low,
        const unsigned long volume) :
    m_date(date),
    m_time(time),
    m_open(open),
    m_close(close),
    m_high(high),
    m_low(low),
    m_volume(volume)
{

}

// Default constructor
TradingDay::TradingDay() :
    m_date(QDate()),
    m_time(0),
    m_open(0),
    m_close(0),
    m_high(0),
    m_low(0),
    m_volume(0)
{

}




