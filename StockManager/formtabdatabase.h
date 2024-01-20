#ifndef FORMTABDATABASE_H
#define FORMTABDATABASE_H

#include <QWidget>
#include <QSettings>
#include <QSettings>

#include "sqlitewrap.h"
#include <QStyledItemDelegate>

class MainWindow;

namespace Ui {
class FormTabDatabase;
}


//==========================================================================================================
//     Class  PaddedItemDelegate

class PaddedItemDelegate : public QStyledItemDelegate
{
public:
    PaddedItemDelegate(int horizontalPadding = 5, int verticalPadding = 2, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), horizontalPadding(horizontalPadding), verticalPadding(verticalPadding) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyleOptionViewItem optionWithPadding = option;
        optionWithPadding.rect.adjust(horizontalPadding, verticalPadding, -horizontalPadding, -verticalPadding);
        QStyledItemDelegate::paint(painter, optionWithPadding, index);
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size += QSize(2 * horizontalPadding, 2 * verticalPadding);
        return size;
    }

private:
    int horizontalPadding;
    int verticalPadding;
};


//==========================================================================================================
//     Class  DbTableModel

class DbTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DbTableModel(QObject *parent = 0) : QAbstractTableModel(parent) {};

    void setSize(int r, int c) {
        _rows = r;
        _cols = c;
    }

    void setData(const std::vector<std::vector<std::string>>& v_table_data,
                               const std::vector<std::string>& v_table_headers,
                               const std::vector<std::string>& v_table_types);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return _rows; };
    int columnCount(const QModelIndex &parent = QModelIndex()) const override { return _cols; };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    //void sort(int column, Qt::SortOrder order);

private:
    int 	_rows, _cols;
    std::vector<std::vector<std::string>> _v_table_data;
    std::vector<std::string> _v_table_headers;
    std::vector<std::string> _v_table_types;
};


//==========================================================================================================
//     Class  FormTabDatabase

class FormTabDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabDatabase(QSettings* settings, SqliteWrap* db, QWidget *parent = nullptr);
    ~FormTabDatabase();

private:
    void on_db_connection();
    void set_btn_visibility_creation(bool is_created);
    void set_btn_visibility_connection(bool is_connected);

private slots:
    void on_bt_execute_sql_clicked();
    void on_bt_save_db_schema_clicked();
    void on_bt_execute_sql_schema_clicked();
    void on_bt_exists_clicked();
    void on_bt_create_clicked();
    void on_bt_delete_clicked();
    void on_bt_connect_clicked();
    void on_bt_disconnect_clicked();

    void on_cb_tables_views_currentTextChanged(const QString &table_name);
    //void on_cb_tables_views_currentTextChanged_(const QString &table_name);

private:
    Ui::FormTabDatabase *ui = nullptr;
    MainWindow* _mw = nullptr;
    QSettings* _settings = nullptr;
    SqliteWrap* _db = nullptr;
    QString _path_filename;
    DbTableModel* _table_model = nullptr;
};

#endif // FORMTABDATABASE_H
