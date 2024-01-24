#include "yahoofinance.h"
#include "qeventloop.h"

#include <sstream>
#include <curl/curl.h>

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


/**
 * @brief Write callback function for Curl
 * @param content Deliver content pointer
 * @param size Content element bytes size
 * @param nmemb Number of content element
 * @param userdata User data where contents are storaged
 * @return Real buffer size = size * nmemb
 */
size_t YahooFinance::writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    // Append the content to user data
    ((std::string*)userdata)->append(content, size * nmemb);

    // Return the real content size
    return size * nmemb;
}


/**
 * @brief Download the spots CSV file from Yahoo Finance
 * @param symbol Quote symbol
 * @param period1 Begining POSIX timestamp
 * @param period2 Ending POSIX timestamp
 * @param interval Date interval for spots, examples:
 *          daily "1d"
 *          weekly "1wk"
 *          annual "1y"
 * @return CSV file containing the spots
 */

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

std::string YahooFinance::download_csv_curl(const std::string &symbol, const std::string& period1, const std::string& period2, const std::string &interval)
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

    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Write result into the buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (void*)YahooFinance::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);
    }

    return responseBuffer;
}
