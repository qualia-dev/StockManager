#include "formtabsettings.h"
#include "ui_formtabsettings.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"


FormTabSettings::FormTabSettings(QSettings* _settings, SqliteWrap* _db, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormTabSettings)
{
    ui->setupUi(this);
    mw = (MainWindow*) parent;
    m_settings = _settings;
    m_db = _db;

    ui->le_db_path->setText(m_settings->value("db_path").toString());
    ui->le_db_name->setText(m_settings->value("db_name").toString());

    m_path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    if (!m_db->exists(m_path_filename.toStdString()))
    {   // Database file does not exist
        ui->bt_connect->setEnabled(false);
        ui->bt_delete->setEnabled(false);
    }
    else
    {   // Database file exists
        ui->bt_create->setEnabled(false);
    }
}

FormTabSettings::~FormTabSettings()
{
    delete ui;
}

void FormTabSettings::on_bt_exists_clicked()
{
    m_path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    if (m_db->exists(m_path_filename.toStdString()))
        mw->ui->te_log->append("Database file exists: " + m_path_filename);
    else
        mw->ui->te_log->append("Database file does not exist: " + m_path_filename);
}


void FormTabSettings::on_bt_create_clicked()
{
    m_path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    if (!m_db->create_db(m_path_filename.toStdString()))
    {
        mw->ui->te_log->append("Error creating Db: " + m_path_filename);
    }
    else
    {
        ui->bt_connect->setEnabled(true);
        ui->bt_delete->setEnabled(true);
        mw->ui->te_log->append("Database file created: " + m_path_filename);
    }
}


void FormTabSettings::on_bt_delete_clicked()
{
    m_path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    // delete file
    if (!m_db->delete_db(m_path_filename.toStdString()))
    {
        mw->ui->te_log->append("Error deleting Db " + m_path_filename);
    }
    else
    {
        ui->bt_connect->setEnabled(false);
        ui->bt_delete->setEnabled(false);
        ui->bt_create->setEnabled(true);
        mw->ui->te_log->append("Database file deleted: " + m_path_filename);
    }
}


void FormTabSettings::on_bt_connect_clicked()
{
    m_path_filename = m_settings->value("db_path").toString() + "/" + m_settings->value("db_name").toString();

    if (!m_db->connect(m_path_filename.toStdString()))
    {
        mw->ui->te_log->append("Error connecting to Db: " + m_path_filename);
        mw->set_connection_database(false);
    }
    else
    {
        mw->ui->te_log->append("Connected to db: " + m_path_filename);
        mw->set_connection_database(true);
    }
}

