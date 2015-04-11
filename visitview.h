#ifndef VISITVIEW_H
#define VISITVIEW_H

#include <QDialog>
#include <QtSql>
#include <QtAlgorithms>
#include <QMessageBox>
#include "globaldata.h"

namespace Ui {
class VisitView;
}

class VisitView : public QDialog
{
    Q_OBJECT

public:
    explicit VisitView(QWidget *parent = 0, int visit = 0);
    explicit VisitView(QWidget *parent = 0, QString id = 0);
    ~VisitView();
signals:
    void update();
private slots:
    void edit_Clicked();
    void createVisit_Clicked();
    void saveEdit_Clicked();
    void cancelEdit_Clicked();
    void dateChanged(QDate date);
private:
    Ui::VisitView *ui;
    QSqlQuery query;
    int visit;
    QString id;
    QDate date;
    bool dateCheckAvail;
    bool editStatus;
    bool canSave;
    void saveVisit();
    void createVisit();
    void loadVisit(int visit);
    int getVisitNumber(QDate date);
    void setEdit(bool status);
};

#endif // VISITVIEW_H
