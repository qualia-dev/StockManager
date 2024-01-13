#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "sqlitewrap.h"

#include "formtabsettings.h"
#include "formtabdownload.h"

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

public:
    Ui::MainWindow *ui = nullptr;
    QSettings* m_settings = nullptr;
    SqliteWrap* m_db = nullptr;

private:
    FormTabSettings* tab_settings = nullptr;
    FormTabDownload* tab_download = nullptr;

};
#endif // MAINWINDOW_H
