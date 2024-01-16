#include "dlgexecutesql.h"
#include "ui_dlgexecutesql.h"

DlgExecuteSQL::DlgExecuteSQL(SqliteWrap* db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgExecuteSQL)
{
    ui->setupUi(this);
    _db = db;
}

DlgExecuteSQL::~DlgExecuteSQL()
{
    delete ui;
}

void DlgExecuteSQL::on_bt_execute_clicked()
{
    QString sql = ui->te_sql_code->toPlainText();

    if (_db->execute_sql(sql.toStdString()))
    {
        ui->lb_result_execute->setText("Success !");
    }
    else
    {
        ui->lb_result_execute->setText("Error : " + QString::fromStdString(_db->get_last_error()));
    }

}

