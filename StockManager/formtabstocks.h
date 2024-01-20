#ifndef FORMTABSTOCKS_H
#define FORMTABSTOCKS_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>

#include "sqlitewrap.h"
#include "finance/stock.h"

class MainWindow;

namespace Ui {
class FormTabStocks;
}


class StockModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    StockModel(QObject *parent = 0) : QAbstractTableModel(parent) {};

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


class FormTabStocks : public QWidget
{
    Q_OBJECT

public:
    explicit FormTabStocks(SqliteWrap* db, QWidget *parent = nullptr);
    ~FormTabStocks();

    void populate_combobox();
    void set_tvstocks_model_from_db();
    void set_tvstocks_model_data (const std::vector<Stock>& v_stocks);
    std::vector<Stock> filter_stocks ();

private slots:
    void on_le_search_textChanged(const QString &arg1);
    void on_cb_market_category_currentTextChanged(const QString &arg1);
    void on_cb_marketplaces_currentTextChanged(const QString &arg1);

private:
    Ui::FormTabStocks *ui = nullptr;
    MainWindow* _mw = nullptr;
    SqliteWrap* _db = nullptr;
    StockModel* _stocks_model = nullptr;
    std::vector<Stock> _v_stocks;
    QSortFilterProxyModel *sort_proxy_model = nullptr;

};

#endif // FORMTABSTOCKS_H
