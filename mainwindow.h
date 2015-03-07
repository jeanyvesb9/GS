#ifndef MAINWINDOW_H
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
    void tab2_Query_Slot();
    void tab2_ID_textChanged(const QString &arg1);
    void tab2_Next_clicked();
    void tab2_Table_doubleClicked(const QModelIndex &index);

    //Tab3
    void tab3_Query_Slot();
    void tab3_GodSon_ID_textChanged(const QString &arg1);
    void tab3_GodSon_DateCheckBox_stateChanged(bool arg1);
    void tab3_GodSon_CityCheckBox_stateChanged(bool arg1);
    void tab3_GodSon_City_currentIndexChanged(int index);
    void tab3_GodSon_NeighborhoodCheckBox_stateChanged(bool arg1);
    void tab3_Back_clicked();
    void tab3_Next_clicked();
    void tab3_Table_doubleClicked(const QModelIndex &index);
    void tab3_Open();
    void tab3$1_Open_goBack_clicked();

    //Tab4
    void tab4_Back_clicked();
    void tab4_End_clicked();


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
    void tab7_Prepare(bool status);
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
    QList <int> getPlace(bool cityAvail, int ID_City, bool neighAvail, QString Neighborhood_Name);
    void disableMenus(bool status);
    bool menuDisabled;
    int previousTab;

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

    //Tab2
    QSqlQueryModel *tab2_model;
    QString tab2_GodParent;
    void tab2_Query();
    void tab2_Erase();

    //Tab3
    QSqlQueryModel *tab3_model;
    QString tab3_GodSon;
    QStringList tab3_neighborhoods;
    void tab3_Query();
    void tab3_Erase();

    //Tab4
    void tab4_loadData();
    void tab4_saveData();

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
    void tab7_disconnect(QString index);


};

#endif // MAINWINDOW_H
