#ifndef SQLCONSOLE_H
#define SQLCONSOLE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
#include "globaldata.h"

namespace Ui {
class SQLConsole;
}

class SQLConsole : public QDialog
{
    Q_OBJECT

public:
    explicit SQLConsole(QWidget *parent = 0);
    ~SQLConsole();
private slots:
    void execute();
    void ok_clicked();
private:
    Ui::SQLConsole *ui;
    QSqlQuery *query;
    QSqlQueryModel *queryModel;
};

#endif // SQLCONSOLE_H
