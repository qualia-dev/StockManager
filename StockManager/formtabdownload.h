#ifndef FORMTABDOWNLOAD_H
#define FORMTABDOWNLOAD_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

#include <QWidget>

#include "sqlitewrap.h"
#include "ftpdownloader.h"

class MainWindow;

namespace Ui {
class FormTabDownload;
}

class FormTabDownload : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabDownload(SqliteWrap* _db, QWidget *parent = nullptr);
    ~FormTabDownload();

private slots:
    void on_bt_dwnNasdaqSecurities_clicked();
    void on_bt_extractSecurities_clicked();

private:
    void dwn_nasdaq_securities_list();

private:
    Ui::FormTabDownload *ui;
    MainWindow* mw;
    SqliteWrap* db;

};

#endif // FORMTABDOWNLOAD_H
