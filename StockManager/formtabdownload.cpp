#include <QFile>
#include <iostream>

#include "formtabdownload.h"
#include "finance/yahoofinance.h"
#include "ui_formtabdownload.h"
#include "finance/marketnasdaq.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"


// ====================================================================================================
//     class FormTabDownload

FormTabDownload::FormTabDownload(SqliteWrap* db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabDownload)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    _db = db;
}

FormTabDownload::~FormTabDownload()
{
    delete ui;
}

void FormTabDownload::on_bt_dwnNasdaqSecurities_clicked()
{
    std::string server = "ftp.nasdaqtrader.com";
    unsigned int port = 21;
    std::string remoteFilePath = "/symboldirectory/nasdaqlisted.txt";   // Remote file path on the FTP server
    std::string localFilePath = "../data/nasdaqlisted.txt";             // Local file path to save the downloaded file TODO : config file

    MarketNasdaq mn(_db);
    if (!mn.dwnd_list_securities(server, port, remoteFilePath, localFilePath))
    {
        std::cout << "Failed to download file from FTP server: " << remoteFilePath << std::endl;
        _mw->ui->te_log->append("Failed to download file from FTP server: " + QString::fromUtf8(server+remoteFilePath));
        return;
    }

    std::cout << "Downloaded file from FTP server: " << remoteFilePath << std::endl;
    _mw->ui->te_log->append("File downloaded successfully from FTP server: " + QString::fromUtf8(server+remoteFilePath));

    _mw->refresh_stocks();
}

void FormTabDownload::on_bt_extractSecurities_clicked()
{
    MarketNasdaq mn(_db);
    if (!mn.extract_securities_from_file())
    {
        std::cout << "Failed to extract securities infos" << std::endl;
        _mw->ui->te_log->append("Failed to extract securities infos");
    }

    std::cout << "File extraction ok" << std::endl;
    _mw->ui->te_log->append("File extraction ok !");

    _mw->refresh_stocks();
}


void FormTabDownload::on_bt_download_tool_url_clicked()
{
    QString url = ui->le_url_tool_download->text();

    QNetworkAccessManager manager;
    QNetworkRequest request((QUrl(url)));
    QNetworkReply *reply = manager.get(request);

    // Setup an event loop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check for errors
    if (reply->error() == QNetworkReply::NoError) {
        // Read the downloaded data
        QByteArray data = reply->readAll();
        QString content(data);
        _mw->ui->te_log->append("Downloaded data:" + content);
        // Process the content as needed
        qDebug() << "Downloaded data:" << content;
    } else {
        // Handle errors
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
}


void FormTabDownload::on_bt_yahoo_send_clicked()
{
    std::string period = "2020-12-01";
    YahooFinance yf;
    std::string content = yf.download_csv("MSFT", period, period, "1d");
    _mw->ui->te_log->append("Downloaded data:" + QString::fromStdString(content));
}


void FormTabDownload::on_bt_nasdaq100_compo_clicked()
{
    MarketNasdaq mn(_db);
    if (!mn.dwnd_nasdaq100_components())
    {
        std::cout << "Failed to download file from FTP server: " << std::endl;
        _mw->ui->te_log->append("Failed to download Nasdaq100 components");
        return;
    }
    _mw->refresh_indexes();
}


void FormTabDownload::on_bt_nasdaq_company_infos_clicked()
{
    MarketNasdaq mn(_db);
    // Get the list of all Nasdaq stocks Nasdaq from database
    std::vector<Stock> v_stocks;
    if (!_mw->get_stocks_from_db("marketplace_name = 'Nasdaq'", v_stocks))
    {
        std::cout << "FormTabDownload::on_bt_nasdaq_company_infos_clicked() - Failed to get Nasdaq stocks from database" << std::endl;
        _mw->ui->te_log->append("Failed to get Nasdaq stocks from database");
        return;
    }

    // Download company infos for each Nasdaq stock
    for (const Stock& stock : v_stocks)
    {
        if (!mn.dwnd_company_profile(stock.symbol()))
        {
            std::cout << "FormTabDownload::on_bt_nasdaq_company_infos_clicked() - Failed to download company infos" << std::endl;
            _mw->ui->te_log->append("Failed to download Nasdaq company infos");
        }
    }

    _mw->refresh_stocks();

    _mw->ui->te_log->append("Downloaded Nasdaq company infos");
}

