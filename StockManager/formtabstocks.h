#ifndef FORMTABSTOCKS_H
#define FORMTABSTOCKS_H

#include <QWidget>

#include "qabstractitemmodel.h"

#include "sqlitewrap.h"
#include "stock.h"

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

    static int select_stock_callback(void* user_param, int nb_rows, char** row_values, char** row_names);

private:
    Ui::FormTabStocks *ui;
    MainWindow* _mw;
    SqliteWrap* _db;
    StockModel* _stocks_model;
    std::vector<Stock> _v_stocks;
    int _expected_rows = -1;

signals:
    void signal_select_stock_complete();

private slots:
    void on_select_stock_complete();
};

#endif // FORMTABSTOCKS_H
