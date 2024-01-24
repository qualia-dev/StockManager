#ifndef YAHOOFINANCE_H
#define YAHOOFINANCE_H

#include <string>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

class YahooFinance
{
public:
    YahooFinance();

    static size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata);

    std::string download_csv(const std::string &symbol, const std::string& period1, const std::string& period2, const std::string &interval);


};

#endif // YAHOOFINANCE_H
