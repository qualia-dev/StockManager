
#ifndef TRADINGDAY_H
#define TRADINGDAY_H

#include "date.h"

class TradingDay
{

public:
    // Create a trading day with the starting values passed
    TradingDay(const Date& date, const float time, const float open, const float close, const float high, const float low, const unsigned long volume);

    // Construct: Empty constructor, default values
    TradingDay();

private:

    //the date of the trading day
    Date m_date;

    // The time of the trade (guess at format)
    float m_time;

    //price of the stock at the start of the trading day
    float m_open;

    //price of the stock at the end of the trading day
    float m_close;

    //price of the stock at it's highest point during the trading day
    float m_high;

    //price of the stock at it's lowest point during the trading day
    float m_low;

    //number of stocks traded that day
    unsigned long int m_volume;

public:

    //getters
    Date date() const {return m_date;}
    float time() const {return m_time;}
    float open() const {return m_open;}
    float close() const {return m_close;}
    float high() const {return m_high;}
    float low() const {return m_low;}
    unsigned long volume() const {return m_volume;}

    //setters
    void date(const Date& date) {m_date = date;}
    void time(const float time) {m_time = time;}
    void open(const float open) {m_open = open;}
    void close(const float close) {m_close = close;}
    void high(const float high) {m_high = high;}
    void low(const float low) {m_low = low;}
    void volume(const unsigned long volume) {m_volume = volume;}
};


#endif // TRADINGDAY_H
