#ifndef FORMTABINDEXES_H
#define FORMTABINDEXES_H

#include <QWidget>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include "sqlitewrap.h"
#include "finance/index.h"
#include "finance/stock.h"

class MainWindow;

namespace Ui {
class FormTabIndexes;
}

// ====================================================================================================
//     class IndexCompoProxyModel : order by column

// class IndexCompoProxyModel : public QSortFilterProxyModel
// {
//     Q_OBJECT
// public:
//     IndexCompoProxyModel(QObject *parent = 0) : QSortFilterProxyModel(parent) {};

//     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
// };



// ====================================================================================================
//     class IndexCompoModel

class IndexCompoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    IndexCompoModel(QObject *parent = 0) : QAbstractTableModel(parent) {};

    void setSize(int r, int c) {
        _rows = r;
        _cols = c;
    }

    void setData(const std::vector<Stock>& people);

    int rowCount(const QModelIndex &parent = QModelIndex()) const { return _rows; };
    int columnCount(const QModelIndex &parent = QModelIndex()) const { return _cols; };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //void sort(int column, Qt::SortOrder order);

private:
    int 	_rows, _cols;
    std::vector<Stock> _v_stocks;
};


// ====================================================================================================
//     class FormTabIndexes

class FormTabIndexes : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabIndexes(SqliteWrap* db, QWidget *parent = nullptr);
    ~FormTabIndexes();

    void populate_combobox();
    void refresh_tvindexcompo_from_db();
    void set_tvindexcompo_model_data (const std::vector<Stock>& _v_compo_stocks);

private slots:
    void on_cb_indexes_currentTextChanged(const QString &arg1);

private:
    Ui::FormTabIndexes *ui;
    MainWindow* _mw = nullptr;
    SqliteWrap* _db = nullptr;
    IndexCompoModel* _indexcompo_model = nullptr;
    std::vector<Index> _v_indexes;        // for combobox
    std::vector<Stock> _v_compo_stocks;
    QSortFilterProxyModel *sort_proxy_model = nullptr;
};

#endif // FORMTABINDEXES_H
