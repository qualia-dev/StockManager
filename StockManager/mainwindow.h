#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "qlabel.h"
#include "sqlitewrap.h"
#include "finance/marketplace.h"
#include "finance/company.h"

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

public:
    bool is_db_connected() { return _db_connected; }
    void get_marketplaces_from_db(std::vector<Marketplace> &v_marketplaces);
    void get_stocks_from_db(std::vector<Stock> &v_stocks);
    void refresh_stocks();
    void set_connection_database(bool connected);

private:
    void init_settings();
    void create_tabs();
    void set_statusbar_text(const QString& text);

private slots:
    void toggleLogSplitter();

public:
    Ui::MainWindow *ui = nullptr;

private:
    QSettings* _settings = nullptr;
    SqliteWrap* _db = nullptr;
    bool _db_connected = false;

    std::vector<Marketplace> _v_marketplaces;
    std::vector<Company> _v_companies;
    std::vector<Stock> _v_stocks;

    QLabel* lb_status_icon = nullptr;
    QLabel* lb_status_text = nullptr;
    FormTabSettings* tab_settings = nullptr;
    FormTabDownload* tab_download = nullptr;
    FormTabDatabase* tab_database = nullptr;
    FormTabStocks* tab_stocks = nullptr;


};
#endif // MAINWINDOW_H
