

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

    if (!_mw->is_db_connected()) return;

    // set the qtableview
    ui->tv_stocks->setAlternatingRowColors(true);
    ui->tv_stocks->horizontalHeader()->setFixedHeight(20);          // set the height of the columns header
    ui->tv_stocks->verticalHeader()->setDefaultSectionSize(20);     // set the heights of the cells
    ui->tv_stocks->setSortingEnabled(true);

    refresh_tvstocks_from_db();   // create model, set the data, populate combo boxes

    // set original columns size and keep the interactive capability
    ui->tv_stocks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tv_stocks->setColumnWidth(0, 50);
    ui->tv_stocks->setColumnWidth(1, 350);
    ui->tv_stocks->setColumnWidth(2, 100);
    ui->tv_stocks->setColumnWidth(3, 200);
    ui->tv_stocks->setColumnWidth(4, 200);
}

FormTabStocks::~FormTabStocks()
{
    delete ui;
}

void FormTabStocks::populate_combobox ()
{
    // get the different values for category field in _v_stocks
    ui->cb_market_category->clear();
    ui->cb_market_category->addItem("All");
    std::vector<std::string> v_market_categories;
    for (const Stock& stock : _v_stocks) {
        if (std::find(v_market_categories.begin(), v_market_categories.end(), stock.marketCategory()) == v_market_categories.end()) {
            v_market_categories.push_back(stock.marketCategory());
            ui->cb_market_category->addItem(QString::fromStdString(stock.marketCategory()));
        }
    }
    // get the different values for market field in _v_stocks
    ui->cb_marketplaces->clear();
    ui->cb_marketplaces->addItem("All");
    std::vector<std::string> v_marketplaces;
    for (const Stock& stock : _v_stocks) {
        if (std::find(v_marketplaces.begin(), v_marketplaces.end(), stock.marketplaceName()) == v_marketplaces.end()) {
            v_marketplaces.push_back(stock.marketplaceName());
            ui->cb_marketplaces->addItem(QString::fromStdString(stock.marketplaceName()));
        }
    }
}


void FormTabStocks::refresh_tvstocks_from_db()
{
    _v_stocks.clear();
    _mw->get_stocks_from_db(_v_stocks);
    populate_combobox();
    set_tvstocks_model_data (_v_stocks);
}


void FormTabStocks::set_tvstocks_model_data (const std::vector<Stock>& v_stocks)
{
    delete (_stocks_model);
    _stocks_model = new StockModel(this);
    _stocks_model->setSize(v_stocks.size(), 5);
    sort_proxy_model = new QSortFilterProxyModel(this);
    sort_proxy_model->setDynamicSortFilter(true);
    sort_proxy_model->setSourceModel(_stocks_model);
    _stocks_model->setData(v_stocks);
    ui->tv_stocks->setModel(sort_proxy_model);
    ui->lb_nb_records->setText(QString::number(v_stocks.size()) + " records");
    ui->tv_stocks->sortByColumn(1, Qt::SortOrder::AscendingOrder);
}


void FormTabStocks::on_le_search_textChanged(const QString &arg1)
{
    set_tvstocks_model_data(filter_stocks());
}


void FormTabStocks::on_cb_market_category_currentTextChanged(const QString &arg1)
{
    set_tvstocks_model_data(filter_stocks());
}


void FormTabStocks::on_cb_marketplaces_currentTextChanged(const QString &arg1)
{
    set_tvstocks_model_data(filter_stocks());
}


std::vector<Stock> FormTabStocks::filter_stocks ()
{
    // TODO : use QSortFilterProxyModel Class instead of this

    QString marketplace = ui->cb_marketplaces->currentText().trimmed();
    QString market_category = ui->cb_market_category->currentText().trimmed();
    QString search_text = ui->le_search->text().trimmed().toLower();
    std::vector<Stock> v_search_results;

    for (const Stock &stock : _v_stocks) {
        QString stock_marketplace_name_lower = QString::fromStdString(stock.marketplaceName());
        QString stock_market_category_lower = QString::fromStdString(stock.marketCategory());
        QString stock_name_lower = QString::fromStdString(stock.name()).toLower();
        QString stock_symbol_lower = QString::fromStdString(stock.symbol()).toLower();

        bool isMarketplaceMatch = (marketplace == "All") || stock_marketplace_name_lower.contains(marketplace);
        bool isMarketCategoryMatch = (market_category == "All") || stock_market_category_lower.contains(market_category);
        bool isSearchMatch = stock_name_lower.contains(search_text) || stock_symbol_lower.contains(search_text);

        if (isMarketplaceMatch && isMarketCategoryMatch && isSearchMatch) {
            v_search_results.push_back(stock);
        }
    }

    return v_search_results;
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
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QVariant();
}


void StockModel::setData(const std::vector<Stock>& stocks)
{
    _v_stocks = stocks;

    // Notify the view that the data has changed
    // This ensures that the view updates itself with the new data
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(_rows - 1, _cols - 1);
    emit dataChanged(topLeft, bottomRight);
}

QVariant StockModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        // Provide header text for each column
        switch (section) {
        case 0:
            return tr("Id");
        case 1:
            return tr("Name");
        case 2:
            return tr("Symbol");
        case 3:
            return tr("Market");
        case 4:
            return tr("Type");
        default:
            return QVariant();
        }

        if ((role == Qt::ToolTipRole) && (orientation == Qt::Horizontal)) {

        }

        if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
            return section + 1;
        }
    }

    return QVariant();
}






