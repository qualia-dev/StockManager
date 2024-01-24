#include "formtabsettings.h"
#include "ui_formtabsettings.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"


FormTabSettings::FormTabSettings(QSettings* settings, SqliteWrap* db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabSettings)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    _settings = settings;
    _db = db;

    ui->le_db_path->setText(_settings->value("db_path").toString());
    ui->le_db_name->setText(_settings->value("db_name").toString());
}

FormTabSettings::~FormTabSettings()
{
    delete ui;
}



