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
    explicit FormTabSettings(QSettings* settings, SqliteWrap* db, QWidget *parent = nullptr);
    ~FormTabSettings();

private slots:

private:
    Ui::FormTabSettings *ui = nullptr;
    MainWindow* _mw = nullptr;
    QSettings* _settings = nullptr;
    SqliteWrap* _db = nullptr;
};

#endif // FORMTABSETTINGS_H


