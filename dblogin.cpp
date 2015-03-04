#include "dblogin.h"
#include "ui_dblogin.h"

DBLogin::DBLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBLogin)
{
    //Login window init
    ui->setupUi(this);
    qDebug() << exePath.absolutePath();
    this->setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/DatabaseLogin_Icon.png")));
    this->setWindowTitle("Iniciar Base de Datos");
    this->setFixedSize(440,100);
}

DBLogin::~DBLogin()
{
    delete ui;
}

void DBLogin::on_pushButton_3_clicked()
{
    //Change DB button
    QString filename = QFileDialog::getOpenFileName(
                this, "Abrir Base de Datos", QString(),
                "Archivo  DB de SQLite (*.db);;Todos los archivos (*)");
    //Sets database directory
    settings.setValue("main/databaseDirectory", filename);
    emit SQLConnect();

}
void DBLogin::connected(bool dbconnected)
{
    ui->DBDirectory->setText(settings.value("main/databaseDirectory", "No seleccionado").toString());
    if (dbconnected)
    {
        connectionStat = true;
        ui->connectionStatus->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600; color:#005500;\">Conectado a Base de Datos</span></p></body></html>");
    }
    else
    {
        connectionStat = false;
         ui->connectionStatus->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:600; color:#ff0000;\">Archivo Inválido</span></p></body></html>");
    }
}

void DBLogin::on_pushButton_2_clicked()
{
    //OK button
    if (connectionStat == false)
    {
        QMessageBox quit(
                    QMessageBox::Warning,
                    "Error",
                    "El archivo seleccionadoo no corresponde a una Base de Datos de SQLite válida",
                    QMessageBox::Yes | QMessageBox::No);
        quit.setButtonText(QMessageBox::Yes, "Reintentar");
        quit.setButtonText(QMessageBox::No, "Salir");
        quit.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Warning_Icon.png")));

        if (quit.exec() == QMessageBox::No)
        {
            close();
        }
    }
    else
    {
        close();
    }
}
void DBLogin::closeEvent(QCloseEvent *event)
{
    if (!connectionStat)
    {
//FIX THIS: PROGRAM CRASHING AFTER CLOSING
        this->parent()->deleteLater();
    }
    event->accept();
}
