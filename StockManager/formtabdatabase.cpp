#include <iostream>

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
        return QString::fromStdString(*(_v_table_data[index.row()][col]));
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }

    return QVariant();
}


void DbTableModel::setData(const std::vector<std::vector<std::unique_ptr<std::string>>>& v_table_data,
                           const std::vector<std::unique_ptr<std::string>>& v_table_headers,
                           const std::vector<std::unique_ptr<std::string>>& v_table_types)
{
    // PROBLEM TO FIX : copy of unique_ptr
    // _v_table_data = v_table_data;
    // _v_table_headers = v_table_headers;
    // _v_table_types = v_table_types;

    // Clear the internal vectors
    _v_table_data.clear();
    _v_table_headers.clear();
    _v_table_types.clear();

    // Copy the data from the input vectors
    for (auto& row : v_table_data)
    {
        std::vector<std::unique_ptr<std::string>> v_row;
        for (auto& col : row)
        {
            v_row.push_back(std::make_unique<std::string>(*col));
        }
        _v_table_data.push_back(std::move(v_row));
    }

    for (auto& header : v_table_headers)
    {
        _v_table_headers.push_back(std::make_unique<std::string>(*header));
    }

    for (auto& type : v_table_types)
    {
        _v_table_types.push_back(std::make_unique<std::string>(*type));
    }


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
            if (section >= 0 && section < _v_table_headers.size())   return QString::fromStdString(*(_v_table_headers[section]));
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


void FormTabDatabase::on_db_connection()
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
}


void FormTabDatabase::on_cb_tables_views_currentTextChanged(const QString &table_name)
{
    // get table content in vector of vector of std::string
    std::vector<std::vector<std::tuple<std::unique_ptr<std::string>, std::unique_ptr<std::string>, std::unique_ptr<std::string>>>> table_content;

    if (!_db->get_table_content(table_name.toStdString(), table_content))
    {
        _mw->ui->te_log->append("Error loading table content");
        return;
    }

    // Build the structured vectors for the model
    std::vector<std::vector<std::unique_ptr<std::string>>> v_table_data;
    std::vector<std::unique_ptr<std::string>> v_table_headers;
    std::vector<std::unique_ptr<std::string>> v_table_types;

    bool first_row = true;
    for (auto& row : table_content)
    {
        std::vector<std::unique_ptr<std::string>> v_row;
        std::string log_row = "";
        for (auto& col : row)
        {
            v_row.push_back(std::move(std::get<2>(col)));
            log_row += *v_row.back() + " - ";

            if (first_row)
            {
                v_table_headers.push_back(std::move(std::get<0>(col)));
                v_table_types.push_back(std::move(std::get<1>(col)));
            }
        }
        std::cout << log_row << std::endl;
        _mw->ui->te_log->append(QString::fromStdString(log_row));
        v_table_data.push_back(std::move(v_row));
        first_row = false;
    }

    // set model
    _table_model = new DbTableModel(this);
    _table_model->setSize(v_table_data.size(), v_table_data[0].size());

    _table_model->setData(v_table_data, v_table_headers, v_table_types);
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
    ui->bt_execute_sql_schema->setEnabled(is_connected);
    ui->bt_execute_sql->setEnabled(is_connected);
}


void FormTabDatabase::on_bt_connect_clicked()
{
    if (!_db->connect(_path_filename.toStdString()))
    {
        _mw->ui->te_log->append("Error connecting to Db: " + _path_filename);
        _mw->set_connection_database(false);
    }
    else
    {
        _mw->ui->te_log->append("Connected to db: " + _path_filename);
        _mw->set_connection_database(true);
        set_btn_visibility_connection(true);
    }
}


void FormTabDatabase::on_bt_disconnect_clicked()
{
    if (_db->disconnect())
    {
        _mw->ui->te_log->append("Disconnected from db: " + _path_filename);
        _mw->set_connection_database(false);
        set_btn_visibility_connection(false);
    }
    else
    {
        _mw->ui->te_log->append("Error disconnecting from Db: " + _path_filename);
    }
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


void FormTabDatabase::on_bt_execute_sql_clicked()
{
    DlgExecuteSQL dlg(_db, this);
    dlg.setModal(true);
    dlg.exec();
}



