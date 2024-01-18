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


void FormTabDatabase::on_bt_save_db_schema_clicked()
{
    if (_db->get_database_schema("../db/rebuild_db_schema.sql"))
    {
        _mw->ui->te_log->append("Database schema saved to file: ../db/rebuild_db_schema.sql");
    }
    else
    {
        _mw->ui->te_log->append("Error saving database schema to file: ../db/rebuild_db_schema.sql");
    }
}


void FormTabDatabase::on_bt_execute_sql_schema_clicked()
{
    if (_db->execute_sql_schema("../db/rebuild_db_schema.sql"))
    {
        _mw->ui->te_log->append("Database schema executed from file: ../db/rebuild_db_schema.sql");
    }
    else
    {
        _mw->ui->te_log->append("Error executing database schema from file: ../db/rebuild_db_schema.sql");
    }
}

