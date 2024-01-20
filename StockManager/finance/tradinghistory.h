
#ifndef TRADINGHISTORY_H
#define TRADINGHISTORY_H

#include <list>
#include <string>
#include "tradingday.h"


class TradingHistory
{
public:

    // Constructor: Create a trading history for the specified first and last trading days
    // Does not load any data
    TradingHistory(
            const QDate& firstTradingDayInData,
            const QDate& lastTradingDayInData);


private:

    // Has the data been loaded from the database
    bool m_loaded;

    // List of trading day data
    std::list<TradingDay> m_tradingData;

    // If the trading day iterator has reached the end of the trading day list, then false.
    // Otherwise true.
    bool m_tradingDataItValid;

    // stores position of the current node from m_tradingData
    std::list<TradingDay>::iterator m_tradingDataIt;

    // The first date that the trading day list has stock data for, on this particular stock.
    QDate m_firstTradingDayInData;

    // The last date that the trading day list has stock data for, on this particular stock.
    QDate m_lastTradingDayInData;

    // Getter for date of first day of trading
    QDate firstDate() const;

    // Getter for date of last day of trading
    QDate lastDate() const;


public:

    // Adds the passed trading day data to the list of trading days
    // Ensure new data is added to list in chronological order
    // Update m_firstTradingDayInData and m_lastTradingDayInData if the new data extends
    // the date range of the list
    // Returns true if succesfully added new day data
    bool addTradingDayData(TradingDay newDayData);

    // Reset at start of list, and copy first item in the list
    // into the parameter.  Return true if success, false otherwise
    bool getFirstTradingDayData(TradingDay& tradingDayData);

    // Advance the iterator, and if there is another element
    // then copy element into the parameter.
    // Return true if success, false otherwise
    bool getNextTradingDayData(TradingDay& tradingDayData);

    // Return number of trading days held in this object
    unsigned long days() const;

    // Setter for trading history loaded
    void loaded(const bool isLoaded);



};

#endif // TRADINGHISTORY_H
