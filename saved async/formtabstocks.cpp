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

    connect(this, &FormTabStocks::signal_select_stock_complete, this, &FormTabStocks::onSelectStockComplete);

    _expected_rows = -1;
    bool rc = _db->select(table, condition , (void*)this, &FormTabStocks::select_stock_callback, _expected_rows);
    if (!rc)
    {
        std::cerr << "FormTabStocks::FormTabStocks - Error: select failed." << std::endl;
        _mw->ui->te_log->append("Error: select failed.");
        return;
    }
}

FormTabStocks::~FormTabStocks()
{
    delete ui;
}


int FormTabStocks::select_stock_callback(void* user_param, int nb_cols, char** col_values, char** col_names)
{
    FormTabStocks* form = (FormTabStocks*) user_param;

    std::string s = "";
    for (int i = 0; i < nb_cols; i++)   s += " | " + std::string(col_names[i]) + ": " + (col_values[i] ? col_values[i] : "NULL");
    std::cout << "cols: " << nb_cols << " - " << s << std::endl;

    Stock stock;
    if (!stock.deserialize (col_values, nb_cols))
    {
        std::cerr << "Error: stock.deserialize failed." << std::endl;
        return 1;
    }

    form->_v_stocks.push_back(stock);

    if (form->_v_stocks.size() == form->_expected_rows)
        emit (form->signal_select_stock_complete());    // select complete

    return 0;
}

void FormTabStocks::onSelectStockComplete()
{
    _mw->ui->te_log->append("select_stock_complete");

    ui->tv_stocks->setAlternatingRowColors(true);
    //ui->tv_stocks->horizontalHeader()->setFixedHeight(40);
    //ui->tv_stocks->horizontalHeader()->setVisible(true);

    // create model
    _stocks_model = new StockModel(this);
    _stocks_model->setSize(_expected_rows, 5);
    _stocks_model->setData(_v_stocks);
    ui->tv_stocks->setModel(_stocks_model);

    // to set an original column size and keep the interactive capability
    ui->tv_stocks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tv_stocks->setColumnWidth(0, 50);
    ui->tv_stocks->setColumnWidth(1, 250);
    ui->tv_stocks->setColumnWidth(2, 100);
    ui->tv_stocks->setColumnWidth(3, 200);
    ui->tv_stocks->setColumnWidth(4, 200);
}

QVariant StockModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    // if(role == Qt::SizeHintRole)
    // not needed column width is set in the widget
    //
    if(role == Qt::DisplayRole)
    {
        switch(col)
        {
        case 0:
            return  _v_stocks[index.row()].id();
        case 1:
            return  QString::fromStdString(_v_stocks[index.row()].name());
        case 2:
            return  QString::fromStdString(_v_stocks[index.row()].symbol());
        case 3:
            return  QString::fromStdString(_v_stocks[index.row()].marketplaceName());
        case 4:
            return  QString::fromStdString(_v_stocks[index.row()].marketCategory());
        default:
            qDebug() << "Not supposed to happen";
            return QVariant();
        }
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignLeft;
    }

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

