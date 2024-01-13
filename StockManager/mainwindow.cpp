#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <ostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    // set splitter original size
    QList<int> sizes;
    sizes.append(1000);
    sizes.append(100);
    ui->splitter->setSizes(sizes);

    init_settings();

    m_db = new SqliteWrap();

    // create tabs
    ui->tw_main->removeTab(0);
//    tab_stocks = new FormTabStocks(db, this);
//    ui->tw_main->addTab(tab_stocks, "Stocks");
    tab_settings = new FormTabSettings(m_settings, m_db, this);
    ui->tw_main->addTab(tab_settings, "Settings");
    tab_download = new FormTabDownload(m_db, this);
    ui->tw_main->addTab(tab_download, "Download");

    QString path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    if (!m_db->exists(path_filename.toStdString()))
    {   // Database file does not exist
        std::cout << "Database file does not exist: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Database file does not exist: " +path_filename);

        return;
    }

    // Database file exists
    std::cout << "Database file exists: " << path_filename.toStdString() << std::endl;
    ui->te_log->append("Database file exists: " + path_filename);

    if (!m_db->connect(path_filename.toStdString()))
    {   // error connecting to database
        std::cout << "Error connecting to database: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Error connecting to database: " +path_filename);

        return;
    }

    // connected to database
    std::cout << "Connected to database: " << path_filename.toStdString() << std::endl;
    ui->te_log->append("Connected to database: " + path_filename);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_settings()
{
    m_settings = new QSettings("qualia", "stockman");  // store in ~/.config/qualia/memo_app.ini.

    // check in settings if db name and db path exists
    if (!m_settings->contains("db_name"))
    {
        ui->te_log->append("Create settings db_name = stocksman.db");
        m_settings->setValue("db_name", "stocksman.db");
    }
    if (!m_settings->contains("db_path"))
    {
        ui->te_log->append("Create settings db_path = ../db");
        m_settings->setValue("db_path", "../db");
    }
}
