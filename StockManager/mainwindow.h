#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "qlabel.h"
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
    void set_statusbar_text(const QString& text);
    void set_icon_database(bool connected);

private slots:
    void toggleLogSplitter();

public:
    void get_marketplaces_from_db(std::vector<Marketplace> &v_marketplaces);
    void get_stocks_from_db(std::vector<Stock> &v_stocks);

    void refresh_stocks();

public:
    Ui::MainWindow *ui = nullptr;
    QSettings* _settings = nullptr;
    SqliteWrap* _db = nullptr;

    std::vector<Marketplace> _v_marketplaces;
    std::vector<Company> _v_companies;
    std::vector<Stock> _v_stocks;

private:
    QLabel* lb_status_icon = nullptr;
    QLabel* lb_status_text = nullptr;
    FormTabSettings* tab_settings = nullptr;
    FormTabDownload* tab_download = nullptr;
    FormTabDatabase* tab_database = nullptr;
    FormTabStocks* tab_stocks = nullptr;


};
#endif // MAINWINDOW_H
