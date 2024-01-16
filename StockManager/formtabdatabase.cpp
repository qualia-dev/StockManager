#include "formtabdatabase.h"
#include "dlgexecutesql.h"
#include "ui_formtabdatabase.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

FormTabDatabase::FormTabDatabase(SqliteWrap* db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabDatabase)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    _db = db;}

FormTabDatabase::~FormTabDatabase()
{
    delete ui;
}

void FormTabDatabase::on_bt_execute_sql_clicked()
{
    DlgExecuteSQL dlg(_db, this);
    dlg.setModal(true);
    dlg.exec();
}

