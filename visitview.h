#ifndef VISITVIEW_H
#define VISITVIEW_H

#include <QDialog>
#include <QtSql>
#include "globaldata.h"

namespace Ui {
class VisitView;
}

class VisitView : public QDialog
{
    Q_OBJECT

public:
    explicit VisitView(QWidget *parent = 0);
    void loadVisit(QString visit);
    ~VisitView();

private:
    Ui::VisitView *ui;
    QSqlQuery query;
    void saveVisit();
};

#endif // VISITVIEW_H
