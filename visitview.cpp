#include "visitview.h"
#include "ui_visitview.h"

VisitView::VisitView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VisitView)
{
    ui->setupUi(this);
    ui->Edit->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Edit_Icon.png")));
    ui->Ok->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Ok_Icon.png")));
}

VisitView::~VisitView()
{
    delete ui;
}

void VisitView::loadVisit(QString visit)
{
    query.prepare("SELECT ");
}
