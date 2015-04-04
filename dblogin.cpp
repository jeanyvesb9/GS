#include "dblogin.h"
#include "ui_dblogin.h"

DBLogin::DBLogin(bool &conStatus, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBLogin)
{
    //Login window init
    ui->setupUi(this);
    this->setWindowIcon(QIcon(iconsPath->absolutePath().append("/DatabaseLogin_Icon.png")));
    this->setWindowTitle("Iniciar Base de Datos");
    this->setFixedSize(440,100);

    conStat = &conStatus;

    connect(ui->Ok, SIGNAL(clicked()), this, SLOT(Ok_clicked()));
    connect(ui->changeDB, SIGNAL(clicked()), this, SLOT(changeDB_clicked()));
    connect(ui->createDB, SIGNAL(clicked()), this, SLOT(createDB_clicked()));
}

DBLogin::~DBLogin()
{
    delete ui;
}

void DBLogin::changeDB_clicked()
{
    //Change DB button
    QString filename = QFileDialog::getOpenFileName(
                this, "Abrir Base de Datos", QString(),
                "Archivo  DB de SQLite (*.db);;Todos los archivos (*)");
    //Sets database directory
    settings->setValue("main/databaseDirectory", filename);
    emit SQLConnect();

}
void DBLogin::connected(bool dbconnected)
{
    ui->DBDirectory->setText(settings->value("main/databaseDirectory", "No seleccionado").toString());
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

void DBLogin::Ok_clicked()
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
        quit.setWindowIcon(QIcon(iconsPath->absolutePath().append("/Warning_Icon.png")));

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

void DBLogin::createDB_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                "Guardar Base de Datos", QString(),
                "Archivo DB de SQLite (*.db)");

    if(QFile::copy(exePath->absolutePath().append("/resources/dbmodel/model.db"), filename))
    {
        QMessageBox open(
                    QMessageBox::Question,
                    "Abrir",
                    "Desea abrir la Base de Datos ahora?",
                    QMessageBox::Yes | QMessageBox::No);
        open.setButtonText(QMessageBox::Yes, "Si");
        open.setWindowIcon(QIcon(iconsPath->absolutePath().append("/Ok_Icon.png")));

        if (open.exec() == QMessageBox::Yes)
        {
            settings->setValue("main/databaseDirectory", filename);
            this->close();
        }
    }
    else
    {
        QMessageBox error(
                    QMessageBox::Warning,
                    "Error",
                    "Error al copiar el archivo",
                    QMessageBox::Ok);
        error.setWindowIcon(QIcon(iconsPath->absolutePath().append("/Warning_Icon.png")));
    }
}

void DBLogin::closeEvent(QCloseEvent * e)
{
    if (!connectionStat)
    {
        *conStat = false;
        return;
    }

    *conStat = true;
}
