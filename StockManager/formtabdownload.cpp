#include <QFile>
#include <iostream>

#include "formtabdownload.h"
#include "ui_formtabdownload.h"
#include "marketnasdaq.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

FormTabDownload::FormTabDownload(SqliteWrap* _db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabDownload)
{
    ui->setupUi(this);
    mw = (MainWindow*) parent;
    db = _db;
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

    MarketNasdaq mn;
    if (!mn.dwnd_list_securities(server, port, remoteFilePath, localFilePath))
    {
        std::cout << "Downloaded file from FTP server: " << remoteFilePath << std::endl;
        mw->ui->te_log->append("File downloaded successfully from FTP server: " + QString::fromUtf8(server+remoteFilePath));
    }
    else
    {
        std::cout << "Failed to download file from FTP server: " << remoteFilePath << std::endl;
        mw->ui->te_log->append("Failed to download file from FTP server: " + QString::fromUtf8(server+remoteFilePath));
    }
}

void FormTabDownload::on_bt_extractSecurities_clicked()
{
    MarketNasdaq mn;
    if (!mn.extract_securities_from_file())
    {
        std::cout << "File extraction ok" << std::endl;
        mw->ui->te_log->append("File extraction ok !");
    }
    else
    {
        std::cout << "Failed to extract securities infos" << std::endl;
        mw->ui->te_log->append("Failed to extract securities infos");
    }
}

