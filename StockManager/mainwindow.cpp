#include <iostream>
#include <ostream>

#include <QShortcut>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define PROJECT_NAME "stockmanager"  // used for the settings file (linux: ~/.config/qualia/PROJECT_NAME.conf)
                                     // and for the name of the database file PROJECT_NAME.db
#define DB_FILE_PATH "../db"         // path to the database file

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showMaximized();

    // set splitter original size
    QList<int> sizes;
    sizes.append(1000);
    sizes.append(10);
    ui->splitter->setSizes(sizes);

    // toggleLogSplitter();  // Hide log panel

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::toggleLogSplitter);

    lb_status_icon = new QLabel(this);
    ui->statusbar->addWidget(lb_status_icon);
    lb_status_text = new QLabel(this);
    ui->statusbar->addWidget(lb_status_text);

    init_settings();

    _db = new SqliteWrap();

    QString path_filename = _settings->value("db_path").toString() + "/" + _settings->value("db_name").toString();

    if (!_db->exists(path_filename.toStdString()))
    {   // Database file does not exist
        std::cout << "Database file does not exist: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Database file does not exist: " +path_filename);
        set_connection_database(false);
    }
    else
    {
        if (!_db->connect(path_filename.toStdString()))
        {   // error connecting to database
            std::cout << "Error connecting to database: " << path_filename.toStdString() << std::endl;
            ui->te_log->append("Error connecting to database: " +path_filename);
            set_connection_database(false);
            return;
        }

        // connected to database
        std::cout << "Connected to database: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Connected to database: " + path_filename);
        set_connection_database(true);
        set_statusbar_text("hello world !");
    }

    // create tabs
    create_tabs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::create_tabs()
{
    ui->tw_main->removeTab(0);
    tab_stocks = new FormTabStocks(_db, this);
    ui->tw_main->addTab(tab_stocks, "Stocks");
    tab_indexes = new FormTabIndexes(_db, this);
    ui->tw_main->addTab(tab_indexes, "Indexes");
    tab_database = new FormTabDatabase(_settings, _db, this);
    ui->tw_main->addTab(tab_database, "Database");
    tab_download = new FormTabDownload(_db, this);
    ui->tw_main->addTab(tab_download, "Download");
    tab_settings = new FormTabSettings(_settings, _db, this);
    ui->tw_main->addTab(tab_settings, "Settings");
}

void MainWindow::set_statusbar_text(const QString &text)
{
    lb_status_text->setText(text);
}

void MainWindow::set_connection_database(bool connected)
{
    if (connected)
        lb_status_icon->setPixmap(QPixmap(":/ButtonIcons/icons/db-connected.png"));
    else
        lb_status_icon->setPixmap(QPixmap(":/ButtonIcons/icons/db-notconnected.png"));

    lb_status_icon->setScaledContents(true);
    lb_status_icon->setMaximumSize(20, 20);
    _db_connected = connected;
}

void MainWindow::init_settings()
{
    _settings = new QSettings("qualia", PROJECT_NAME);  // store in ~/.config/qualia/stockmanager.conf

    // check in settings if db name and db path exists
    if (!_settings->contains("db_name"))
    {
        QString file_name = QString(PROJECT_NAME)  + ".db";            // db file : stockmanager.db
        ui->te_log->append("Create settings db_name = " + file_name);
        _settings->setValue("db_name", file_name);
    }
    if (!_settings->contains("db_path"))
    {
        ui->te_log->append("Create settings db_path = " + QString(DB_FILE_PATH));
        _settings->setValue("db_path", QString(DB_FILE_PATH));
    }
}


void MainWindow::refresh_stocks()
{
    tab_stocks->refresh_tvstocks_from_db();
}

void MainWindow::refresh_indexes()
{
    tab_indexes->refresh_tvindexcompo_from_db();
}

void MainWindow::toggleLogSplitter()
{
    // Assuming logSplitter is the name of your QSplitter
    if (ui->te_log->isVisible())
    {
        ui->te_log->hide();
        ui->splitter->setSizes({ui->splitter->width(), 0 });
    }
    else
    {
        ui->splitter->setSizes({ 1, 10 });
        ui->te_log->show();
    }
}

// ======================================================================================
//   Database Access
// ======================================================================================

void MainWindow::get_marketplaces_from_db(std::vector<Marketplace> &v_marketplaces)
{
    std::string table = "MarketplaceView";
    std::string condition = "";

    bool rc = _db->select_sync(table, condition, (void*)&v_marketplaces, Marketplace::deserialize);

    for (auto &mkp : v_marketplaces)
    {
        std::cout << "id: " << mkp.id() << " - name: " << mkp.name() << " - name_full: " << mkp.name_full() << " - country_id: " << mkp.countryId() << " - country: " << mkp.country() << std::endl;
    }
}



bool MainWindow::get_stocks_from_db(std::vector<Stock> &v_stocks)
{
    std::string table = "V_Stocks";
    std::string condition = "";
    return _db->select_sync(table, condition, (void*)&v_stocks, Stock::deserialize);
}

bool MainWindow::get_stocks_from_db(const std::string condition, std::vector<Stock> &v_stocks)
{
    std::string table = "V_Stocks";
    return _db->select_sync(table, condition, (void*)&v_stocks, Stock::deserialize);
}

bool MainWindow::get_indexes_from_db(std::vector<Index> &v_indexes)
{
    std::string table = "V_Indexes";  // View
    std::string condition = "";
    return _db->select_sync(table, condition, (void*)&v_indexes, Index::deserialize);
}

bool MainWindow::get_index_composition(const std::string condition, std::vector<Stock> &v_compo_stock)
{
    std::string table = "V_IndexComposition";  // View
    return _db->select_sync(table, condition, (void*)&v_compo_stock, Stock::deserialize_compo_index);
}



