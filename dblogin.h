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
    explicit DBLogin(bool &conStatus, QWidget *parent = 0);
    ~DBLogin();
    inline bool getConectionStat() { return connectionStat; }

private slots:
    void changeDB_clicked();
    void Ok_clicked();
    void createDB_clicked();
public slots:
    void connected(bool);
signals:
    void SQLConnect();
protected:
    void closeEvent(QCloseEvent * e);
private:
    Ui::DBLogin *ui;
    bool connectionStat;
    bool *conStat;
};

#endif // DBLOGIN_H
