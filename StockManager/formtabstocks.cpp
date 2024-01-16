#include "formtabstocks.h"
#include "ui_formtabstocks.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"



FormTabStocks::FormTabStocks(SqliteWrap* db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabStocks)
{
    ui->setupUi(this);
    _mw = (MainWindow*) parent;
    _db = db;

    // get stocks from db
    std::string table = "StockView";
    std::string condition = "";
    std::string result = "";

    connect(this, &FormTabStocks::signal_select_stock_complete, this, &FormTabStocks::on_select_stock_complete);

    _expected_rows = -1;
    bool rc = _db->select_count_async(table, condition , _expected_rows);
    rc = _db->select(table, condition , (void*)this, &FormTabStocks::select_stock_callback, result);

}

FormTabStocks::~FormTabStocks()
{
    delete ui;
}


int FormTabStocks::select_stock_callback(void* user_param, int nb_cols, char** col_values, char** col_names)
{
    FormTabStocks* form = (FormTabStocks*) user_param;

    std::cout << "stock_callback - nb_rows: " << nb_cols << std::endl;
    std::string s = "";
    for (int i = 0; i < nb_cols; i++)
    {
        s += " | " + std::string(col_names[i]) + ": " + (col_values[i] ? col_values[i] : "NULL");
    }
    std::cout << s << std::endl;

    Stock stock;
    if (!stock.deserialize (col_values, nb_cols))
    {
        std::cerr << "Error: stock.deserialize failed." << std::endl;
        return 1;
    }

    form->_v_stocks.push_back(stock);

    if (form->_v_stocks.size() == form->_expected_rows)
    {
        // select complete
        emit (form->signal_select_stock_complete());
    }

    return 0;
}

void FormTabStocks::on_select_stock_complete()
{
    _mw->ui->te_log->append("select_stock_complete");
}

QVariant StockModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    // if(role == Qt::SizeHintRole)
    // {
    //     switch(col)
    //     {
    //     case 0:
    //         return QSize(150, 10);
    //     case 1:
    //         return QSize(150, 10);
    //     case 2:
    //         return QSize(150, 10);
    //     case 3:
    //         return QSize(250, 10);
    //     case 4:
    //         return QSize(250, 10);
    //     }
    // }
    // else if(role == Qt::DisplayRole)
    // {
    //     switch(col)
    //     {
    //     case 0:
    //         return  QString::fromStdString(_v_people[index.row()].firstName);
    //     case 1:
    //         return  QString::fromStdString(_v_people[index.row()].lastName);
    //     case 2:
    //         return  QString::fromStdString(_v_people[index.row()].sex);
    //     case 3:
    //         return  QString::fromStdString(_v_people[index.row()].email);
    //     case 4:
    //         return  QString::fromStdString(_v_people[index.row()].phone);
    //     default:
    //         qDebug() << "Not supposed to happen";
    //         return QVariant();
    //     }
    // }
    // else if(role == Qt::TextAlignmentRole)
    // {
    //     return Qt::AlignCenter;
    // }

    return QVariant();
}


void StockModel::setData(const std::vector<Stock>& stocks)
{
    // Assuming that v_people is a member of your PersonModel
    _v_stocks = stocks;

    // Notify the view that the data has changed
    // This ensures that the view updates itself with the new data
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(_rows - 1, _cols - 1);
    emit dataChanged(topLeft, bottomRight);
}

QVariant StockModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    //     // Provide header text for each column
    //     switch (section) {
    //     case 0:
    //         return tr("First Name");
    //     case 1:
    //         return tr("Last Name");
    //     case 2:
    //         return tr("Sex");
    //     case 3:
    //         return tr("Email");
    //     case 4:
    //         return tr("Phone");
    //     default:
    //         return QVariant();
    //     }

    //     if ((role == Qt::ToolTipRole) && (orientation == Qt::Horizontal)) {

    //     }

    //     if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
    //         return section + 1;
    //     }
    // }

    return QVariant();
}

