#ifndef FORMTABSETTINGS_H
#define FORMTABSETTINGS_H

#include <QWidget>
#include <QSettings>

#include "sqlitewrap.h"

class MainWindow;

namespace Ui {
class FormTabSettings;
}

class FormTabSettings : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabSettings(QSettings* _settings, SqliteWrap* _db, QWidget *parent = nullptr);
    ~FormTabSettings();

private slots:
    void on_bt_exists_clicked();
    void on_bt_create_clicked();
    void on_bt_delete_clicked();
    void on_bt_connect_clicked();

private:
    Ui::FormTabSettings *ui = nullptr;
    MainWindow* mw = nullptr;
    QSettings* m_settings = nullptr;
    SqliteWrap* m_db = nullptr;
    QString m_path_filename;
};

#endif // FORMTABSETTINGS_H


