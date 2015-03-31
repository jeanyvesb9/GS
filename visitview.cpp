#include "visitview.h"
#include "ui_visitview.h"

VisitView::VisitView(QWidget *parent, int visit) :
    QDialog(parent),
    ui(new Ui::VisitView),
    visit{visit}
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(iconPath->absolutePath().append("/Open_Icon.png")));

    loadVisit(visit);
    setEdit(false);

}

VisitView::VisitView(QWidget *parent, QString id) :
    QDialog(parent),
    ui(new Ui::VisitView),
    id{id}
{
    ui->setupUi(this);
    ui->Godson_ID->setText(id);
    ui->Date->setDate(QDate::currentDate());

    setEdit(true);
    ui->Ok->setText("Crear");
    ui->Ok->setIcon(QIcon(iconPath->absolutePath().append("/Ok_Icon.png")));
    ui->VisitNumber->setText(QString::number(getVisitNumber(QDate::currentDate())));
    this->setWindowTitle("Crear Visita");

    disconnect(ui->Edit, 0, 0, 0);
    connect(ui->Ok, SIGNAL(clicked()), this, SLOT(createVisit_Clicked()));
    //TODO emit signal when closed, no creation
    disconnect(ui->Ok, 0, 0, 0);
    connect(ui->Edit, SIGNAL(clicked()), this, SLOT(close()));
}

VisitView::~VisitView()
{
    delete ui;
}

int VisitView::getVisitNumber(QDate date)
{
    query.clear();
    query.prepare("SELECT Visits.Date FROM Visits WHERE Visits.ID_GodsonCode LIKE :id");
    query.bindValue(":id", id);
    query.exec();

    QList<QDate> dateList;
    while (query.next())
    {
        dateList.append(QDate::fromString(query.value("Date").toString(), "dd/MM/yyyy"));
    }
    qSort(dateList);

    if (dateList.contains(date))
        return -1;

    int i = 0;
    for (; i < dateList.size(); i++)
    {
        if (dateList[i] > date)
        {
            return i + 1;
        }
    }
    return i + 1;
}

void VisitView::loadVisit(int visit)
{
    query.clear();
    query.prepare("SELECT Visits. ID_GodsonCode, Visits.Date, Visits.Meeting, Visits.GeneralStatus, Visits.Comment FROM Visits WHERE Visits.ID_Visit = :id");
    query.bindValue(":id", visit);
    query.exec();
    query.next();

    ui->Godson_ID->setText(query.value("ID_GodsonCode").toString());
    ui->Date->setDate(QDate::fromString(query.value("Date").toString(), "dd/MM/yyyy"));
    ui->Meeting->setChecked(query.value("Meeting").toBool());
    ui->GeneralStatus->setCurrentIndex(query.value("GeneralStatus").toInt());
    ui->Comment->setPlainText(query.value("Comment").toString());

    ui->VisitNumber->setText(QString::number(getVisitNumber(QDate::fromString(query.value("Date").toString(), "dd/MM/yyyy"))));
}

void VisitView::saveVisit()
{
    query.clear();
    query.prepare("UPDATE Visits SET Date = :date, Meeting = :meeting, GeneralStatus = :gs, Comment = :comment WHERE Visits.ID_Visit = :id");
    query.bindValue(":id", visit);
    query.bindValue(":date", ui->Date->date().toString("dd/MM/yyyy"));
    query.bindValue(":meeting", ui->Meeting->isChecked());
    query.bindValue(":gs", ui->GeneralStatus->currentIndex());
    query.bindValue(":comment", ui->Comment->toPlainText());

    query.exec();
}

void VisitView::createVisit()
{
    query.prepare("INSERT INTO Visits(ID_GodsonCode, Date, Meeting, GeneralStatus, Comment) VALUES (:id, :date, :meeting, :gs, :comment)");
    query.bindValue(":id", id);
    query.bindValue(":date", ui->Date->date().toString("dd/MM/yyyy"));
    query.bindValue(":meeting",ui->Meeting->isChecked());
    query.bindValue(":gs", ui->GeneralStatus->currentIndex());
    query.bindValue(":comment", ui->Comment->toPlainText());

    query.exec();
}

void VisitView::edit_Clicked()
{
    setEdit(!editStatus);
}

void VisitView::createVisit_Clicked()
{
    if (canSave)
    {
    createVisit();
    this->close();
    return;
    }
    QMessageBox *message = new QMessageBox;
    message->setWindowIcon(QIcon(iconPath->absolutePath().append("/Warning_Icon.png")));
    message->setText("La fecha ingresada corresponde a una visita existente, o supera la fecha del Sistema. Por favor corrobore la configuración de Windows.");
    message->show();
}

void VisitView::saveEdit_Clicked()
{
    if (canSave)
    {
        saveVisit();
        setEdit(false);
        return;
    }
    QMessageBox *message = new QMessageBox;
    message->setWindowIcon(QIcon(iconPath->absolutePath().append("/Warning_Icon.png")));
    message->setText("La fecha ingresada corresponde a una visita existente, o supera la fecha del Sistema. Por favor corrobore la configuración de Windows.");
    message->show();
}

void VisitView::cancelEdit_Clicked()
{
    loadVisit(visit);
    canSave = 1;
    setEdit(false);
}

void VisitView::dateChanged(QDate date)
{
    int dateNumber = getVisitNumber(date);
    QPalette *palette = new QPalette();

    if (dateNumber == -1 || date > QDate::currentDate())
    {
        palette->setColor(QPalette::Text, Qt::red);
        ui->VisitNumber->setText("-");
        canSave = 0;
    }
    else
    {
        palette->setColor(QPalette::Text, Qt::black);
        ui->VisitNumber->setText(QString::number(dateNumber));
        canSave = 1;
    }
    ui->Date->setPalette(*palette);
}

void VisitView::setEdit(bool status)
{
    editStatus = status;

    ui->VisitNumber->setReadOnly(true);
    ui->Godson_ID->setReadOnly(true);
    ui->Date->setReadOnly(!status);
    ui->Meeting->setEnabled(status);
    ui->GeneralStatus->setEnabled(status);
    ui->Comment->setReadOnly(!status);

    if (status)
    {
        ui->Date->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        disconnect(ui->Edit, 0, 0, 0);
        connect(ui->Edit, SIGNAL(clicked()), this, SLOT(cancelEdit_Clicked()));
        ui->Edit->setText("Cancelar");
        ui->Edit->setIcon(QIcon(iconPath->absolutePath().append("/Warning_Icon.png")));
        disconnect(ui->Ok, 0, 0, 0);
        connect(ui->Ok, SIGNAL(clicked()), this, SLOT(saveEdit_Clicked()));
        ui->Ok->setText("Guardar");
        ui->Ok->setIcon(QIcon(iconPath->absolutePath().append("/Save_Icon.png")));
        this->setWindowTitle("Editar Visita");

        connect(ui->Date, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged(QDate)));
    }
    else
    {
        ui->Date->setButtonSymbols(QAbstractSpinBox::NoButtons);
        disconnect(ui->Edit, 0, 0, 0);
        connect(ui->Edit, SIGNAL(clicked()), this, SLOT(edit_Clicked()));
        ui->Edit->setText("Editar");
        ui->Edit->setIcon(QIcon(iconPath->absolutePath().append("/Edit_Icon.png")));
        disconnect(ui->Ok, 0, 0, 0);
        connect(ui->Ok, SIGNAL(clicked()), this, SLOT(close()));
        ui->Ok->setText("Ok");
        ui->Ok->setIcon(QIcon(iconPath->absolutePath().append("/Ok_Icon.png")));
        this->setWindowTitle("Ver Visita");

        disconnect(ui->Date, 0, 0, 0);
    }
}
