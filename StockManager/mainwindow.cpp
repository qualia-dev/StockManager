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

    _db = new SqliteWrap();

    QString path_filename = _settings->value("db_path").toString() + "/" + _settings->value("db_name").toString();

    if (!_db->exists(path_filename.toStdString()))
    {   // Database file does not exist
        std::cout << "Database file does not exist: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Database file does not exist: " +path_filename);

        return;
    }

    // Database file exists
    std::cout << "Database file exists: " << path_filename.toStdString() << std::endl;
    ui->te_log->append("Database file exists: " + path_filename);

    if (!_db->connect(path_filename.toStdString()))
    {   // error connecting to database
        std::cout << "Error connecting to database: " << path_filename.toStdString() << std::endl;
        ui->te_log->append("Error connecting to database: " +path_filename);

        return;
    }

    // connected to database
    std::cout << "Connected to database: " << path_filename.toStdString() << std::endl;
    ui->te_log->append("Connected to database: " + path_filename);

    // get_database_data();

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
    tab_database = new FormTabDatabase(_db, this);
    ui->tw_main->addTab(tab_database, "Database");
    tab_settings = new FormTabSettings(_settings, _db, this);
    ui->tw_main->addTab(tab_settings, "Settings");
    tab_download = new FormTabDownload(_db, this);
    ui->tw_main->addTab(tab_download, "Download");
}

void MainWindow::init_settings()
{
    _settings = new QSettings("qualia", "stockman");  // store in ~/.config/qualia/memo_app.ini.

    // check in settings if db name and db path exists
    if (!_settings->contains("db_name"))
    {
        ui->te_log->append("Create settings db_name = stocksman.db");
        _settings->setValue("db_name", "stocksman.db");
    }
    if (!_settings->contains("db_path"))
    {
        ui->te_log->append("Create settings db_path = ../db");
        _settings->setValue("db_path", "../db");
    }
}


// ======================================================================================
//   The following code is working but it s not good practice to load all data at startup
// ======================================================================================

void MainWindow::get_database_data()
{
    // get marketplaces from db
    std::string table = "MarketplaceView";
    std::string condition = "";
    std::string result = "";

    connect(this, &MainWindow::signal_select_marketplace_complete, this, &MainWindow::onSelectMarketplaceComplete);

    _expected_rows = -1;
    bool rc = _db->select(table, condition , (void*)this, &MainWindow::select_marketplace_callback, _expected_rows);
    if (!rc)
    {
        std::cerr << "MainWindow::MainWindow - Error: select failed." << std::endl;
        ui->te_log->append("Error: select failed.");
        return;
    }
}

int MainWindow::select_marketplace_callback(void* user_param, int nb_cols, char** col_values, char** col_names)
{
    std::cout << "select_marketplace_callback" << std::endl;

    MainWindow* mw = (MainWindow*) user_param;

    std::string s = "";
    for (int i = 0; i < nb_cols; i++)   s += " | " + std::string(col_names[i]) + ": " + (col_values[i] ? col_values[i] : "NULL");
    std::cout << "cols: " << nb_cols << " - " << s << std::endl;

    Marketplace marketplace;
    if (!marketplace.deserialize (col_values, nb_cols))
    {
        std::cerr << "Error: marketplace.deserialize failed." << std::endl;
        return 1;
    }

    mw->_v_marketplaces.push_back(marketplace);

    if (mw->_v_marketplaces.size() == mw->_expected_rows)
    {
        std::cout << "select marketplace complete - emit signal" << std::endl;
        emit (mw->signal_select_marketplace_complete());    // select complete
    }

    return 0;
}

void MainWindow::onSelectMarketplaceComplete()
{
    std::cout << "select marketplace complete - slot for signal" << std::endl;
    ui->te_log->append("select_marketplace_complete");

    // get companies from db
    std::string table = "Company";
    std::string condition = "";
    std::string result = "";

    connect(this, &MainWindow::signal_select_company_complete, this, &MainWindow::onSelectCompanyComplete);

    _expected_rows = -1;
    bool rc = _db->select(table, condition , (void*)this, &MainWindow::select_company_callback, _expected_rows);
    if (!rc)
    {
        std::cerr << "MainWindow::MainWindow - Error: select failed." << std::endl;
        ui->te_log->append("Error: select failed.");
        return;
    }
}

int MainWindow::select_company_callback(void* user_param, int nb_cols, char** col_values, char** col_names)
{
    std::cout << "select_company_callback" << std::endl;

    MainWindow* mw = (MainWindow*) user_param;

    std::string s = "";
    for (int i = 0; i < nb_cols; i++)   s += " | " + std::string(col_names[i]) + ": " + (col_values[i] ? col_values[i] : "NULL");
    std::cout << "cols: " << nb_cols << " - " << s << std::endl;

    Company company;
    if (!company.deserialize (col_values, nb_cols))
    {
        std::cerr << "Error: company.deserialize failed." << std::endl;
        return 1;
    }

    mw->_v_companies.push_back(company);

    if (mw->_v_companies.size() == mw->_expected_rows)
    {
        std::cout << "select company complete - emit signal" << std::endl;
        emit (mw->signal_select_company_complete());    // select complete
    }

    return 0;
}

void MainWindow::onSelectCompanyComplete()
{
    std::cout << "select company complete - slot for signal" << std::endl;
    ui->te_log->append("select_company_complete");

    // get stocks from db
    std::string table = "StockView";
    std::string condition = "";
    std::string result = "";

    connect(this, &MainWindow::signal_select_stock_complete, this, &MainWindow::onSelectStockComplete);

    _expected_rows = -1;
    bool rc = _db->select(table, condition , (void*)this, &MainWindow::select_stock_callback, _expected_rows);
    if (!rc)
    {
        std::cerr << "MainWindow::MainWindow - Error: select failed." << std::endl;
        ui->te_log->append("Error: select failed.");
        return;
    }
}

int MainWindow::select_stock_callback(void* user_param, int nb_cols, char** col_values, char** col_names)
{
    std::cout << "select_stock_callback" << std::endl;

    MainWindow* mw = (MainWindow*) user_param;

    std::string s = "";
    for (int i = 0; i < nb_cols; i++)   s += " | " + std::string(col_names[i]) + ": " + (col_values[i] ? col_values[i] : "NULL");
    std::cout << "cols: " << nb_cols << " - " << s << std::endl;

    Stock stock;
    if (!stock.deserialize (col_values, nb_cols))
    {
        std::cerr << "Error: stock.deserialize failed." << std::endl;
        return 1;
    }

    mw->_v_stocks.push_back(stock);

    if (mw->_v_stocks.size() == mw->_expected_rows)
    {
        std::cout << "select stock complete - emit signal" << std::endl;
        emit (mw->signal_select_stock_complete());    // select complete
    }

    return 0;
}

void MainWindow::onSelectStockComplete()
{
    std::cout << "select stock complete - slot for signal" << std::endl;
    ui->te_log->append("select_stock_complete");
}


