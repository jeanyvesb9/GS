#ifndef DBLOGIN_H
#define DBLOGIN_H

#include <QDialog>
#include <QCloseEvent>
#include <QFileDialog>
#include <QSettings>
#include <QString>
#include <QSql>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>
#include "globaldata.h"

namespace Ui {
class DBLogin;
}

class DBLogin : public QDialog
{
    Q_OBJECT

public:
    DBLogin(QWidget *parent = 0);
    ~DBLogin();
    bool getConectionStat() { return connectionStat; }

private slots:
    void changeDB_clicked();
    void OK_clicked();
public slots:
    void connected(bool);
signals:
    void SQLConnect();
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::DBLogin *ui;
    bool connectionStat;
};

#endif // DBLOGIN_H
