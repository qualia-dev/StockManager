#ifndef FORMTABDATABASE_H
#define FORMTABDATABASE_H

#include <QWidget>

#include "sqlitewrap.h"

class MainWindow;

namespace Ui {
class FormTabDatabase;
}

class FormTabDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabDatabase(SqliteWrap* _db, QWidget *parent = nullptr);
    ~FormTabDatabase();

private slots:
    void on_bt_execute_sql_clicked();

    void on_bt_save_db_schema_clicked();

    void on_bt_execute_sql_schema_clicked();

private:
    Ui::FormTabDatabase *ui = nullptr;
    MainWindow* _mw = nullptr;
    SqliteWrap* _db = nullptr;
};

#endif // FORMTABDATABASE_H
