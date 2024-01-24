#include "formtabindexes.h"
#include "ui_formtabindexes.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"



// ====================================================================================================
//     class IndexCompoModel

QVariant IndexCompoModel::data(const QModelIndex &index, int role) const
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
            return  QString::fromStdString(_v_stocks[index.row()].marketCategory());
        case 4:
            return  QString::fromStdString(_v_stocks[index.row()].sector());
        case 5:
            return  QString::fromStdString(_v_stocks[index.row()].industry());
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


void IndexCompoModel::setData(const std::vector<Stock>& stocks)
{
    _v_stocks = stocks;

    // Notify the view that the data has changed => update
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(_rows - 1, _cols - 1);
    emit dataChanged(topLeft, bottomRight);
}


QVariant IndexCompoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return tr("Type");
        case 4:
            return tr("Sector");
        case 5:
            return tr("Industry");
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



// ====================================================================================================
//     class FormTabIndexes

FormTabIndexes::FormTabIndexes(SqliteWrap *db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabIndexes)
{
    ui->setupUi(this);

    _mw = (MainWindow*) parent;
    _db = db;

    ui->tv_compo_stocks->setAlternatingRowColors(true);
    ui->tv_compo_stocks->horizontalHeader()->setFixedHeight(20);          // set the height of the columns header
    ui->tv_compo_stocks->verticalHeader()->setDefaultSectionSize(20);     // set the heights of the cells
    ui->tv_compo_stocks->setSortingEnabled(true);

    if (!_db->is_connected())
        return;

    _mw->get_indexes_from_db(_v_indexes);
    populate_combobox();

    // set original columns size and keep the interactive capability
    ui->tv_compo_stocks->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    ui->tv_compo_stocks->setColumnWidth(0, 50);
    ui->tv_compo_stocks->setColumnWidth(1, 350);
    ui->tv_compo_stocks->setColumnWidth(2, 100);
    ui->tv_compo_stocks->setColumnWidth(3, 200);
    ui->tv_compo_stocks->setColumnWidth(4, 200);
    ui->tv_compo_stocks->setColumnWidth(5, 400);
}



FormTabIndexes::~FormTabIndexes()
{
    delete ui;
}


void FormTabIndexes::populate_combobox()
{
    ui->cb_indexes->clear();
    for (const Index& index : _v_indexes) {
        ui->cb_indexes->addItem(QString::fromStdString(index.name()), index.id());
    }
}


void FormTabIndexes::refresh_tvindexcompo_from_db()
{
    _v_compo_stocks.clear();
    int index_id = ui->cb_indexes->currentData().toInt();
    std::string condition = "index_id = " + std::to_string(index_id);
    _mw->get_index_composition(condition, _v_compo_stocks);
    set_tvindexcompo_model_data (_v_compo_stocks);
}


void FormTabIndexes::set_tvindexcompo_model_data(const std::vector<Stock> &_v_compo_stocks)
{
    delete _indexcompo_model;
    _indexcompo_model = new IndexCompoModel(this);
    _indexcompo_model->setSize(_v_compo_stocks.size(), 6);
    sort_proxy_model = new QSortFilterProxyModel(this);
    sort_proxy_model->setDynamicSortFilter(true);
    sort_proxy_model->setSourceModel(_indexcompo_model);
    _indexcompo_model->setData(_v_compo_stocks);
    ui->tv_compo_stocks->setModel(sort_proxy_model);
    ui->tv_compo_stocks->sortByColumn(1, Qt::SortOrder::AscendingOrder);  // TODO ordering case sensitive
    ui->lb_nb_records->setText(QString::number(_v_compo_stocks.size()) + " records");
}

void FormTabIndexes::on_cb_indexes_currentTextChanged(const QString &arg1)
{
    refresh_tvindexcompo_from_db();
}

