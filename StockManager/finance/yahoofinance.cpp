#include "yahoofinance.h"
#include "qeventloop.h"

#include <sstream>

YahooFinance::YahooFinance() {}



std::time_t dateToEpoch(const char *date) {
    char year[5] = {0};
    strncpy(year, date, 4);

    char month[3] = {0};
    strncpy(month, date + 5, 2);

    char day[3] = {0};
    strncpy(day, date + 8, 2);

    struct tm time = {0};
    time.tm_year = std::atoi(year) - 1900;
    time.tm_mon = std::atoi(month) - 1;
    time.tm_mday = std::atoi(day);

    return timegm(&time);
}


std::string YahooFinance::download_csv(const std::string &symbol, const std::string& period1, const std::string& period2, const std::string &interval)
{
    std::time_t epoch = dateToEpoch("2020-12-01") + 72000;

    std::stringstream ss1;
    ss1 << epoch;
    std::stringstream ss2;
    ss2 << epoch;

    std::string url = "https://query1.finance.yahoo.com/v7/finance/download/"
                      + symbol
                      + "?period1=" + ss1.str()
                      + "&period2=" + ss2.str()
                      + "&interval=" + interval
                      + "&events=history";

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(QString::fromStdString(url)));
    QNetworkReply *reply = manager.get(request);

    // Setup an event loop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check for errors
    QString content = "";
    if (reply->error() == QNetworkReply::NoError) {
        // Read the downloaded data
        QByteArray data = reply->readAll();
        content = QString::fromUtf8(data);
        // Process the content as needed
        qDebug() << "Downloaded data:" << content;
    } else {
        // Handle errors
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();

    return content.toStdString();
}

