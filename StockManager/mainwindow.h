#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "sqlitewrap.h"
#include "marketplace.h"
#include "company.h"

#include "formtabsettings.h"
#include "formtabdownload.h"
#include "formtabdatabase.h"
#include "formtabstocks.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_settings();
    void create_tabs();
    void get_database_data();

    static int select_marketplace_callback(void* user_param, int nb_rows, char** row_values, char** row_names);
    static int select_company_callback(void* user_param, int nb_rows, char** row_values, char** row_names);
    static int select_stock_callback(void* user_param, int nb_rows, char** row_values, char** row_names);

signals:
    void signal_select_marketplace_complete();
    void signal_select_company_complete();
    void signal_select_stock_complete();

private slots:
    void onSelectMarketplaceComplete();
    void onSelectCompanyComplete();
    void onSelectStockComplete();

public:
    Ui::MainWindow *ui = nullptr;
    QSettings* _settings = nullptr;
    SqliteWrap* _db = nullptr;

private:
    FormTabSettings* tab_settings = nullptr;
    FormTabDownload* tab_download = nullptr;
    FormTabDatabase* tab_database = nullptr;
    FormTabStocks* tab_stocks = nullptr;

    int _expected_rows = -1;

    std::vector<Marketplace> _v_marketplaces;
    std::vector<Company> _v_companies;
    std::vector<Stock> _v_stocks;


};
#endif // MAINWINDOW_H
