#ifndef DLGEXECUTESQL_H
#define DLGEXECUTESQL_H

#include <QDialog>

#include "sqlitewrap.h"

namespace Ui {
class DlgExecuteSQL;
}

class DlgExecuteSQL : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExecuteSQL(SqliteWrap* _db, QWidget *parent = nullptr);
    ~DlgExecuteSQL();

private slots:
    void on_bt_execute_clicked();

private:
    Ui::DlgExecuteSQL *ui;
    SqliteWrap* _db = nullptr;
};

#endif // DLGEXECUTESQL_H
