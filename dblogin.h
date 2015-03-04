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
    explicit DBLogin(QWidget *parent = 0);
    ~DBLogin();
    inline bool getConectionStat() { return connectionStat; }

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
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
