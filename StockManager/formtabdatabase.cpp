#include <iostream>
#include <fstream>

#include <QMessageBox>

#include "formtabdatabase.h"
#include "dlgexecutesql.h"
#include "ui_formtabdatabase.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"




//==========================================================================================================
//     Class  DbTableModel

QVariant DbTableModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();

    if(role == Qt::DisplayRole)
    {
        return QString::fromStdString(_v_table_data[index.row()][col]);
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }

    return QVariant();
}


void DbTableModel::setData(const std::vector<std::vector<std::string>>& v_table_data,
                           const std::vector<std::string>& v_table_headers,
                           const std::vector<std::string>& v_table_types)
{
    // Clear the internal vectors
    _v_table_data.clear();
    _v_table_headers.clear();
    _v_table_types.clear();

    // Copy the data from the input vectors
    for (auto& row : v_table_data)
    {
        std::vector<std::string> v_row;
        for (auto& col : row) v_row.push_back(col);
        _v_table_data.push_back(v_row);
    }

    for (auto& header : v_table_headers) _v_table_headers.push_back(header);
    for (auto& type : v_table_types) _v_table_types.push_back(type);

    // Notify the view that the data has changed
    // This ensures that the view updates itself with the new data
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(_rows - 1, _cols - 1);
    emit dataChanged(topLeft, bottomRight);
}


QVariant DbTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section >= 0 && section < _v_table_headers.size())
            {
                return QString::fromStdString(_v_table_headers[section]);
            }
        }
        else if (orientation == Qt::Vertical)
        {
            return QString::fromStdString(std::to_string(section));
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }

    return QVariant();
}


//==========================================================================================================
//     Class  FormTabDatabase

FormTabDatabase::FormTabDatabase(QSettings* settings, SqliteWrap* db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabDatabase)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    _db = db;
    _settings = settings;

    _path_filename = _settings->value("db_path").toString() + "/" + _settings->value("db_name").toString();

    if (!_db->exists(_path_filename.toStdString()))
    {   // Database file does not exist
        set_btn_visibility_connection(false);
        ui->bt_delete->setEnabled(false);
        set_btn_visibility_creation(false);
        return;
    }

    ui->bt_create->setEnabled(false);    // Database file exists
    set_btn_visibility_connection(_db->is_connected());

    ui->tv_db_content->setAlternatingRowColors(true);
    ui->tv_db_content->horizontalHeader()->setFixedHeight(20);          // set the height of the columns header
    ui->tv_db_content->verticalHeader()->setDefaultSectionSize(20);     // set the heights of the cells

    PaddedItemDelegate* paddedDelegate = new PaddedItemDelegate(15, 0);
    ui->tv_db_content->setItemDelegate(paddedDelegate);

    if (_db->is_connected())
        on_db_connection ();
}


FormTabDatabase::~FormTabDatabase()
{
    delete ui;
}


void FormTabDatabase::on_db_connection(int idx_table_selected)
{
    std::vector<std::string> table_list;
    // get database schema
    if (!_db->get_table_list(table_list))
    {
        _mw->ui->te_log->append("Error loading database table list");
        return;
    }

    // populate combobox
    ui->cb_tables_views->clear();
    for (auto& table_name : table_list)
    {
        if (table_name.substr(0, 7) == "sqlite_") continue;  // remove sqlite internal tables
        ui->cb_tables_views->addItem(QString::fromStdString(table_name));
    }
    if (table_list.size() > 0)
        ui->cb_tables_views->setCurrentIndex(idx_table_selected);
}


void FormTabDatabase::on_cb_tables_views_currentTextChanged(const QString &table_name)
{
    // get table content in vector of vector of std::string
    std::vector<std::vector<std::tuple<std::string, std::string, std::string>>> table_content;

    if (table_name == "") return;

    _mw->ui->te_log->append("Loading table content: " + table_name);

    if (!_db->get_table_content_(table_name.toStdString(), table_content))
    {
        _mw->ui->te_log->append("Error loading table content");
        return;
    }

    // Build the structured vectors for the model
    std::vector<std::vector<std::string>> v_table_data;
    std::vector<std::string> v_table_headers;
    std::vector<std::string> v_table_types;

    bool first_row = true;
    for (auto& row : table_content)
    {
        std::vector<std::string> v_row;
        for (auto& col : row)
        {
            v_row.push_back(std::get<2>(col));

            if (first_row)
            {
                v_table_headers.push_back(std::get<0>(col));
                v_table_types.push_back(std::get<1>(col));
            }
        }
        v_table_data.push_back(v_row);
        first_row = false;
    }

    // set model
    _table_model = new DbTableModel(this);

    if (v_table_data.size() == 0)
        _table_model->setSize(0, 0);
    else
    {
        _table_model->setSize(v_table_data.size(), v_table_data[0].size());
        _table_model->setData(v_table_data, v_table_headers, v_table_types);
    }
    ui->tv_db_content->setModel(_table_model);

    ui->tv_db_content->resizeColumnsToContents();

}


void FormTabDatabase::set_btn_visibility_creation(bool is_created)
{
    ui->bt_create->setEnabled(!is_created);
    ui->bt_connect->setEnabled(is_created);
}


void FormTabDatabase::on_bt_exists_clicked()
{
    if (_db->exists(_path_filename.toStdString()))
        _mw->ui->te_log->append("Database file exists: " + _path_filename);
    else
        _mw->ui->te_log->append("Database file does not exist: " + _path_filename);
}


void FormTabDatabase::on_bt_create_clicked()
{
    if (!_db->create_db(_path_filename.toStdString()))
    {
        _mw->ui->te_log->append("Error creating Db: " + _path_filename);
    }
    else
    {
        set_btn_visibility_connection(false);
        set_btn_visibility_creation(true);
        _mw->ui->te_log->append("Database file created: " + _path_filename);
    }
}


void FormTabDatabase::on_bt_delete_clicked()
{
    // delete file
    if (!_db->delete_db(_path_filename.toStdString()))
    {
        _mw->ui->te_log->append("Error deleting Db " + _path_filename);
    }
    else
    {
        ui->bt_delete->setEnabled(false);
        set_btn_visibility_creation(false);
        _mw->ui->te_log->append("Database file deleted: " + _path_filename);
    }
}


void FormTabDatabase::set_btn_visibility_connection(bool is_connected)
{
    ui->bt_connect->setEnabled(!is_connected);
    ui->bt_delete->setEnabled(!is_connected);
    ui->bt_disconnect->setEnabled(is_connected);
    ui->bt_save_db_schema->setEnabled(is_connected);
    ui->bt_save_table_contaent->setEnabled(is_connected);
    ui->bt_execute_sql_schema->setEnabled(is_connected);
    ui->bt_insert_contaent->setEnabled(is_connected);
    ui->bt_execute_sql->setEnabled(is_connected);
}


void FormTabDatabase::on_bt_connect_clicked()
{
    if (!_db->connect(_path_filename.toStdString()))
    {
        _mw->ui->te_log->append("Error connecting to Db: " + _path_filename);
        _mw->set_connection_database(false);
        return;
    }

    _mw->ui->te_log->append("Connected to db: " + _path_filename);
    _mw->set_connection_database(true);
    set_btn_visibility_connection(true);
    on_db_connection();
}


void FormTabDatabase::on_bt_disconnect_clicked()
{
    if (!_db->disconnect())
    {
        _mw->ui->te_log->append("Error disconnecting from Db: " + _path_filename);
        return;
    }

    _mw->ui->te_log->append("Disconnected from db: " + _path_filename);
    _mw->set_connection_database(false);
    set_btn_visibility_connection(false);
    // reset qtableview
    _table_model = new DbTableModel(this);
    _table_model->setSize(0, 0);
    ui->tv_db_content->setModel(_table_model);
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
    if (!_db->execute_sql_file("../db/rebuild_db_schema.sql"))
    {
        _mw->ui->te_log->append("Error executing database schema from file: ../db/rebuild_db_schema.sql");
        return;
    }

    _mw->ui->te_log->append("Database schema executed from file: ../db/rebuild_db_schema.sql");
    on_db_connection();
}


void FormTabDatabase::on_bt_execute_sql_clicked()
{
    DlgExecuteSQL dlg(_db, this);
    dlg.setModal(true);
    dlg.exec();
}


void FormTabDatabase::on_bt_save_table_contaent_clicked()
{
    std::vector v_tables = {"T_Countries", "T_Indexes", "T_Marketplaces"};
    std::string sql_insert_all_tables = "";

    // iterate through v_tables
    for (const std::string &table_name : v_tables)
    {
        std::vector<std::vector<std::string>> v2_contents;
        _db->select_sync(table_name, "", (void*)&v2_contents, select_callback);

        // generate the sql INSERT command to re-insert the data
        std::string sql = "INSERT INTO " + table_name + " VALUES ";
        for (auto& row : v2_contents)
        {
            sql += "(";
            for (auto& col : row)
            {
                if (col == "NULL") sql += col + ",";
                else sql += "'" + col + "',";
            }
            sql.pop_back();
            sql += "),";
        }
        sql.pop_back();
        sql += ";";

        sql_insert_all_tables += "@@@sql@@@\n" + sql + "\n";
    }

    // save to file : _path_filename/sql_insert_all_tables.sql
    std::string filename = _settings->value("db_path").toString().toStdString() + "/rebuild_all_insert.sql";
    std::ofstream ofs(filename);
    ofs << sql_insert_all_tables;
    ofs.close();

     _mw->ui->te_log->append("Database Rows Data saved to file: ../rebuild_all_insert.sql");
}

bool FormTabDatabase::select_callback (void* user_param, char** cols, int nb_col)
{
    std::vector <std::string>table_row;
    for (int i = 0; i < nb_col; i++)
    {
        // first column is the rowid let the db generates it
        if (i==0)   table_row.push_back("NULL");
        else        table_row.push_back(cols[i]);
    }

    std::vector<std::vector<std::string>>* v2_contents = (std::vector<std::vector<std::string>>*) user_param;
    v2_contents->push_back(table_row);

    return true;
}


void FormTabDatabase::on_bt_insert_contaent_clicked()
{
    std::string filename = _settings->value("db_path").toString().toStdString() + "/rebuild_all_insert.sql";

    if (!_db->execute_sql_file(filename))
    {
        _mw->ui->te_log->append("Error INSERT from file: rebuild_all_insert.sql");
        return;
    }

    _mw->ui->te_log->append("Database INSERT tables executed from file : rebuild_all_insert.sql");
    on_db_connection();
}

void FormTabDatabase::on_tbt_delete_rows_clicked()
{
    // open dialogbox to confirm
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete all rows from table: " + ui->cb_tables_views->currentText());
    msgBox.setInformativeText("This action cannot be undone !");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel) return;

    std::string sql = "DELETE FROM " + ui->cb_tables_views->currentText().toStdString();
    _db->execute_sql(sql);
    // call on_db_connection to refresh the table passing the current index of the combobox
    on_db_connection(ui->cb_tables_views->currentIndex());

    if (ui->cb_tables_views->currentText() == "T_Stocks") _mw->refresh_stocks();
    else if (ui->cb_tables_views->currentText() == "T_Stocks_In_Indexes") _mw->refresh_indexes();

}


void FormTabDatabase::on_tbt_delete_table_clicked()
{
    // open dialogbox to confirm
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete the table: " + ui->cb_tables_views->currentText());
    msgBox.setInformativeText("This action cannot be undone !");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel) return;

    std::string sql = "DROP TABLE " + ui->cb_tables_views->currentText().toStdString();
    _db->execute_sql(sql);
    on_db_connection();

    if (ui->cb_tables_views->currentText() == "T_Stocks")
    {
        _mw->refresh_stocks();
    }
}















