#include "sqlconsole.h"
#include "ui_sqlconsole.h"

SQLConsole::SQLConsole(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLConsole)
{
    ui->setupUi(this);

    ui->console->hide();

    connect(ui->exec, SIGNAL(clicked()), this, SLOT(execute()));
    connect(ui->passwordOk, SIGNAL(clicked()), this, SLOT(ok_clicked()));
    connect(ui->passwordCancel, SIGNAL(clicked()), this, SLOT(close()));

    this->setFixedSize(375, 90);
    this->setWindowIcon(QIcon(iconPath->absolutePath().append("/Console_Icon.png")));
    ui->error->setReadOnly(true);

    query = new QSqlQuery;
    queryModel = new QSqlQueryModel;

}

SQLConsole::~SQLConsole()
{
    delete ui;
}

void SQLConsole::execute()
{
    ui->error->setPlainText("");
    if(!query->exec(ui->query->toPlainText()))
    {
        ui->error->setPlainText(query->lastError().text());
    }
    queryModel->setQuery(*query);
    ui->table->setModel(queryModel);
}

void SQLConsole::ok_clicked()
{
    if (ui->password->text() != "Kalimera007")
    {
        QMessageBox *message = new QMessageBox;
        message->setText("Wrong Password");
        message->setStandardButtons(QMessageBox::Ok);
        message->setWindowTitle("Error");
        message->setWindowIcon(QIcon(iconPath->absolutePath().append("/Warning_Icon.png")));
        message->exec();
        delete message;
        this->close();
    }
    else
    {
        this->setFixedSize(468, 475);
        ui->passwordLogin->hide();
        ui->console->show();
    }
}
