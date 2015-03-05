﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QAction>
#include <QtSql>
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QSettings>
#include <QLabel>
#include <QProgressBar>
#include "dblogin.h"
#include "globaldata.h"
#include "sqlconsole.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    DBLogin *login;
    ~MainWindow();
private slots:
    void on_actionCambiar_Base_de_Datos_triggered();

    void on_actionBuscar_Ahijado_triggered();
    void on_actionApadrinar_triggered();
    void on_actionNuevo_Padrino_triggered();
    void on_actionAdministrar_Padrinos_triggered();
    void on_actionAdministrar_Ahijados_triggered();
    void on_actionImprimir_triggered();

    void actionSQLConsole();

    //TabWidget
    void tabWidget_indexChanged(int index);

    //Tab0
    void tab0_Query_Slot();
    void tab0_GodSon_ID_textChanged(const QString &arg1);
    void tab0_GodSon_DateCheckBox_stateChanged(int arg1);
    void tab0_GodSon_CityCheckBox_stateChanged(int arg1);
    void tab0_GodSon_City_currentIndexChanged(int index);
    void tab0_GodSon_NeighborhoodCheckBox_stateChanged(int arg1);
    void tab0_GodParent_ID_textChanged(const QString &arg1);
    void tab0_Erase_clicked();
    void tab0_TableView_doubleClicked(const QModelIndex &index);
    void tab0_SearchOpen_clicked();
    void tab0$1_goBack_clicked();

    //Tab1
    void tab1_edit_clicked();
    void tab1_save_clicked();
    void tab1_basicInfo_City_currentIndexChanged(int index);
    void tab1_fatherDate(bool state);
    void tab1_motherDate(bool state);
    void tab1_ProgramData_Remove();
    void tab1_ProgramData_Cancel();
    void tab1_ProgramData_Accept();
    void tab1_ProgramData_Edit();
    void tab1$7_ProgramData_Edit_goBack_clicked();

    //Tab2
    void tab2_Next_clicked();

    //Tab5
    void tab5_Erase_clicked();
    void tab5_Create_clicked();

    //Tab6
    void tab6_Query_Slot();
    void tab6_GodParent_ID_textChanged(const QString &arg1);
    void tab6_Erase_clicked();
    void tab6_SearchOpen_clicked();
    void tab6_TableView_doubleClicked(const QModelIndex &index);
    void tab6$7_goBack_clicked();

    //Tab7
    void tab7_edit_clicked();
    void tab7_save_clicked();
    void tab7_delete_clicked();
    void tab7_open_clicked();
    void tab7_Godsons_Table_doubleClicked(const QModelIndex &index);
    void tab7_disconnect_clicked();
    void tab7$1_open_goBack_clicked();

public slots:
    void tryConnect();
signals:
    void isSQLConnected(bool);
private:
    Ui::MainWindow *ui;
    QSqlDatabase sqlite;
    QLabel *statLabel;
    QProgressBar *statProgress;
    bool openDB();
    bool isDBOpen;

    //General
    QStringList cities;
    QStringList GeneralStatus;
    QStringList StudyDistance;
    int getAge(QDate date);

    //Tab0
    void tab0_Query();
    QSqlQueryModel *tab0_model;
    QStringList tab0_neighborhoods;

    //Tab1
    QString tab1_searchIndex;
    bool tab1_editMode;
    bool tab1_godParentAvail;
    void tab1_setEditMode(bool status);
    void tab1_loadData();
    void tab1_saveData();
    QStringList tab1_neighborhoods;
    QSqlQueryModel *tab1_model;
    int tab1_ICOid;
    int tab1_NewICOid;
    int tab1_ICOage;
    bool tab1_isRegional;

    //Tab5
    void tab5_erase();

    //Tab6
    void tab6_Query();
    QSqlQueryModel *tab6_model;

    //Tab7
    QString tab7_searchIndex;
    void tab7_loadData();
    void tab7_saveData();
    QSqlQueryModel *tab7_model;
    void tab7_setEditMode(bool status);
    bool tab7_editMode;
    bool tab7_disconnect(QString index);


};

#endif // MAINWINDOW_H
