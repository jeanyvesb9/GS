#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Main Window init
    ui->setupUi(this);
    this->setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/MainWindow_Icon.png")));
    this->setWindowTitle("Gestión Solidaria - Proyecto Sagrada Familia");

    //StatusBar
    statLabel = new QLabel;
    statProgress = new QProgressBar;
    statProgress->setValue(0);
    this->statusBar()->insertWidget(0, statProgress);
    this->statusBar()->insertPermanentWidget(1, statLabel);
    this->statusBar()->setSizeGripEnabled(false);
    this->statProgress->hide();
    this->statusBar()->showMessage("Listo",0);
	
    //Toolbar Actions setup
    QWidget* empty0 = new QWidget();
    empty0->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionBuscar_Ahijado, empty0);

    QWidget* empty1 = new QWidget();
    empty1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionApadrinar, empty1);

    QWidget* empty2 = new QWidget();
    empty2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionNuevo_Padrino, empty2);

    QWidget* empty3 = new QWidget();
    empty3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionAdministrar_Padrinos, empty3);

    QWidget* empty4 = new QWidget();
    empty4->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionAdministrar_Ahijados, empty4);

    QWidget* empty5 = new QWidget();
    empty5->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionImprimir, empty5);

    QWidget* empty6 = new QWidget();
    empty6->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->mainToolBar->addWidget(empty6);

    ui->mainToolBar->insertSeparator(ui->actionImprimir);

    ui->actionBuscar_Ahijado->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Search_Icon.png")));
    ui->actionApadrinar->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/AddRelationship_Icon.png")));
    ui->actionAdministrar_Ahijados->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/AdminAhijados_Icon.png")));
    ui->actionAdministrar_Padrinos->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/AdminPadrinos_Icon.png")));
    ui->actionNuevo_Padrino->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/AddGodParent_Icon.png")));
    ui->actionImprimir->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Print_Icon.png")));

    QActionGroup *toolbarActions = new QActionGroup(this);
    ui->actionBuscar_Ahijado->setActionGroup(toolbarActions);
    ui->actionApadrinar->setActionGroup(toolbarActions);
    ui->actionAdministrar_Ahijados->setActionGroup(toolbarActions);
    ui->actionAdministrar_Padrinos->setActionGroup(toolbarActions);
    ui->actionNuevo_Padrino->setActionGroup(toolbarActions);

    ui->actionBuscar_Ahijado->setCheckable(true);
    ui->actionApadrinar->setCheckable(true);
    ui->actionAdministrar_Ahijados->setCheckable(true);
    ui->actionAdministrar_Padrinos->setCheckable(true);
    ui->actionNuevo_Padrino->setCheckable(true);

    //MenuActions
    connect(ui->actionSQL_Script, SIGNAL(triggered()), this, SLOT(actionSQLConsole()));

    //Define sqlite DB
    sqlite = QSqlDatabase::addDatabase("QSQLITE");
    //Declare and Connect Login
    login = new DBLogin(this);
    connect (login, SIGNAL(SQLConnect()), this, SLOT(tryConnect()));
    connect (this, SIGNAL(isSQLConnected(bool)), login, SLOT(connected(bool)));
    //Check for valid DB
    isDBOpen = openDB();
    //Call for Login Window
    login->setModal(true);
    login->exec();
    delete login;

    //TabWidget
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidget_indexChanged(int)));
    ui->tabWidget->tabBar()->hide();

    GeneralStatus.append("Bueno");
    GeneralStatus.append("Regular");
    GeneralStatus.append("Malo");
    GeneralStatus.append("Muy Malo");

    StudyDistance.append("Local");
    StudyDistance.append("Cercano");
    StudyDistance.append("Lejano");
    StudyDistance.append("Muy Lejano");

    QSqlQuery setData;
    setData.exec("SELECT Name FROM City");
    while(setData.next())
    {
        cities.append(setData.value("name").toString());
    }
    setData.exec("SELECT Name FROM Place WHERE ID_City = 1");
    while(setData.next())
    {
        tab0_neighborhoods.append(setData.value("name").toString());
    }
    ui->tabWidget->setCurrentIndex(0);

        //--------------------------------------------------------------------------
        //Tab0
        connect(ui->tab0_GodSon_ID, SIGNAL(textChanged(QString)), this, SLOT(tab0_GodSon_ID_textChanged(QString)));
        connect(ui->tab0_GodSon_Name, SIGNAL(textChanged(QString)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_GodSon_Surname, SIGNAL(textChanged(QString)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_GodSon_DateCheckBox, SIGNAL(stateChanged(int)), this, SLOT(tab0_GodSon_DateCheckBox_stateChanged(int)));
        connect(ui->tab0_GodSon_Date, SIGNAL(userDateChanged(QDate)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_GodSon_CityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(tab0_GodSon_CityCheckBox_stateChanged(int)));
        connect(ui->tab0_GodSon_City, SIGNAL(currentIndexChanged(int)), this, SLOT(tab0_GodSon_City_currentIndexChanged(int)));
        connect(ui->tab0_GodSon_NeighborhoodCheckBox, SIGNAL(stateChanged(int)), this, SLOT(tab0_GodSon_NeighborhoodCheckBox_stateChanged(int)));
        connect(ui->tab0_GodSon_Neighborhood, SIGNAL(currentTextChanged(QString)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_GodParent_ID, SIGNAL(textChanged(QString)), this, SLOT(tab0_GodParent_ID_textChanged(QString)));
        connect(ui->tab0_GodParent_Name, SIGNAL(textChanged(QString)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_GodParent_Surname, SIGNAL(textChanged(QString)), this, SLOT(tab0_Query_Slot()));
        connect(ui->tab0_Erase, SIGNAL(clicked()), this, SLOT(tab0_Erase_clicked()));
        connect(ui->tab0_TableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tab0_TableView_doubleClicked(QModelIndex)));
        connect(ui->tab0_SearchOpen, SIGNAL(clicked()), this, SLOT(tab0_SearchOpen_clicked()));

        ui->tab0_GodSon_ID->setMaxLength(4);
        ui->tab0_GodSon_City->addItems(cities);
        ui->tab0_GodSon_Neighborhood->addItems(tab0_neighborhoods);
        ui->tab0_GodParent_ID->setMaxLength(4);

        ui->tab0_Erase->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Erase_Icon.png")));
        ui->tab0_SearchOpen->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Open_Icon.png")));

        ui->tab0_TableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tab0_TableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tab0_TableView->setColumnWidth(0, 40);
        ui->tab0_TableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
        ui->tab0_TableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
        ui->tab0_TableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
        ui->tab0_TableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
        ui->tab0_TableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
        ui->tab0_TableView->verticalHeader()->setVisible(false);

        tab0_Query();

        //--------------------------------------------------------------------------
        //Tab1
        connect(ui->tab1_BasicInfo_City, SIGNAL(currentIndexChanged(int)), this, SLOT(tab1_basicInfo_City_currentIndexChanged(int)));
        connect(ui->tab1_goBack, SIGNAL(clicked()), this, SLOT(tab1_goBack_clicked()));
        connect(ui->tab1_edit, SIGNAL(clicked()), this, SLOT(tab1_edit_clicked()));
        connect(ui->tab1_saveChanges, SIGNAL(clicked()), this, SLOT(tab1_save_clicked()));
        connect(ui->tab1_Family_Father_DeathDate_Known, SIGNAL(clicked(bool)), this, SLOT(tab1_fatherDate(bool)));
        connect(ui->tab1_Family_Mother_DeathDate_Known, SIGNAL(clicked(bool)), this, SLOT(tab1_motherDate(bool)));
        connect(ui->tab1_ProgramData_Remove, SIGNAL(clicked()), this, SLOT(tab1_ProgramData_Remove()));
        connect(ui->tab1_ProgramData_To_Cancel, SIGNAL(clicked()), this, SLOT(tab1_ProgramData_Cancel()));
        connect(ui->tab1_ProgramData_To_Accept, SIGNAL(clicked()), this, SLOT(tab1_ProgramData_Accept()));

        QList<QGroupBox*> groupBox_List = ui->tab_searchView->findChildren<QGroupBox*>();
        foreach (QGroupBox* groupBox, groupBox_List)
        {
            groupBox->setStyleSheet("QGroupBox { color : rgb(66,130,196);}");
        }
        ui->tab1_ProgramData->setStyleSheet("QGroupBox { color : rgb(40,46,108); }");
        ui->tab1_ProgramData_GodParent->setStyleSheet("QGroupBox { color : rgb(92,188,103); }");
        foreach(QString item, GeneralStatus)
        {
            ui->tab1_Health_General->addItem(item);
            ui->tab1_Visits_LastVisit_GeneralStatus->addItem(item);
        }
        foreach(QString item, StudyDistance)
        {
            ui->tab1_School_Distance->addItem(item);
        }
        for(int i = 1; i<=12; i++)
        {
            ui->tab1_School_Year->addItem(QString::number(i).append("° Año"));
        }
        ui->tab1_BasicInfo_City->addItems(cities);
        ui->tab1_BasicInfo_Gender->addItem("F");
        ui->tab1_BasicInfo_Gender->addItem("M");
        ui->tab1_edit->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Edit_Icon.png")));
        ui->tab1_saveChanges->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Save_Icon.png")));
        ui->tab1_goBack->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Back_Icon.png")));
        ui->tab1_Visits_Table->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tab1_Visits_Table->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tab1_Visits_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tab1_Visits_Table->verticalHeader()->setVisible(false);

        tab1_setEditMode(false);


        //--------------------------------------------------------------------------
        //Tab2



		//--------------------------------------------------------------------------
		//Tab5
        connect(ui->tab5_Erase, SIGNAL(clicked()), this, SLOT(tab5_Erase_clicked()));
        connect(ui->tab5_Create, SIGNAL(clicked()), this, SLOT(tab5_Create_clicked()));

        ui->tab5_ID->setReadOnly(true);

        //--------------------------------------------------------------------------
        //Tab6
        connect(ui->tab6_GodParent_ID, SIGNAL(textChanged(QString)), this, SLOT(tab6_GodParent_ID_textChanged(QString)));
        connect(ui->tab6_GodParent_Name, SIGNAL(textChanged(QString)), this, SLOT(tab6_Query_Slot()));
        connect(ui->tab6_GodParent_Surname, SIGNAL(textChanged(QString)), this, SLOT(tab6_Query_Slot()));

        connect(ui->tab6_Erase, SIGNAL(clicked()), this, SLOT(tab6_Erase_clicked()));
        connect(ui->tab6_SearchOpen, SIGNAL(clicked()), this, SLOT(tab6_SearchOpen_clicked()));
        connect(ui->tab6_tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tab6_TableView_doubleClicked(QModelIndex)));

        ui->tab6_Erase->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Erase_Icon.png")));
        ui->tab6_SearchOpen->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Open_Icon.png")));

        ui->tab6_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tab6_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tab6_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tab6_tableView->verticalHeader()->setVisible(false);

        tab6_Query();


        //--------------------------------------------------------------------------
        //Tab7





























}
MainWindow::~MainWindow()
{
    sqlite.close();
    delete ui;
}

//Database functions:

bool MainWindow::openDB()
{
    //Get database Address and open Database
    QString databaseAddress = settings.value("main/databaseDirectory","Ninguna").toString();
    if (sqlite.isOpen())
        sqlite.close();
    sqlite.setDatabaseName(databaseAddress);
    QFileInfo DBfileInfo(databaseAddress);
    if (DBfileInfo.suffix() == "db" && sqlite.open())
    {
        this->statLabel->setText(QString("Conectado - ").append(settings.value("main/databaseDirectory").toString()).append(" "));
        emit isSQLConnected(true);
        return true;

    }
    else
    {
        emit isSQLConnected(false);
        return false;
    }
}
void MainWindow::tryConnect()
{
    isDBOpen = openDB();
}

void MainWindow::on_actionCambiar_Base_de_Datos_triggered()
{
    //Archivo/Cambiar Base de Datos

    //Declare and Connect Login
    login = new DBLogin(this);
    connect (login, SIGNAL(SQLConnect()), this, SLOT(tryConnect()));
    connect (this, SIGNAL(isSQLConnected(bool)), login, SLOT(connected(bool)));
    //Check for valid DB
    isDBOpen = openDB();
    //Call for Login Window
    login->setModal(true);
    login->exec();
    delete login;
}

//Menu actions setup:

void MainWindow::on_actionBuscar_Ahijado_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_actionApadrinar_triggered()
{
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_actionNuevo_Padrino_triggered()
{
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::on_actionAdministrar_Padrinos_triggered()
{
    ui->tabWidget->setCurrentIndex(6);
}

void MainWindow::on_actionAdministrar_Ahijados_triggered()
{
    ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Administrar Ahijados</span></p></body></html>");
    ui->tabWidget->setCurrentIndex(8);

}

void MainWindow::on_actionImprimir_triggered()
{
    ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Imprimir</span></p></body></html>");
    ui->tabWidget->setCurrentIndex(9);

}

void MainWindow::actionSQLConsole()
{
    SQLConsole *SQLScriptConsole;
    SQLScriptConsole = new SQLConsole;

    SQLScriptConsole->setModal(true);
    SQLScriptConsole->exec();

    delete SQLScriptConsole;
}

//Tab Initializer:
void MainWindow::tabWidget_indexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Buscar Ahijado</span></p></body></html>");
        break;
    case 1:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Ver ahijado</span></p></body></html>");
        break;
    case 2:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Apadrinar</span></p></body></html>");
        break;
    case 5:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Nuevo Padrino</span></p></body></html>");
        tab5_erase();
        break;
    case 6:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Administrar Padrinos</span></p></body></html>");
        break;
    case 7:
        ui->TitleLabel->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:18pt; font-weight:600; color:#000000;\">Ver Padrino</span></p></body></html>");
        break;


    }
}

//--------------------------------------------------------------------------
//Tab0

void MainWindow::tab0_Query()
{
    tab0_model = new QSqlQueryModel;
    QSqlQuery query;

    QString ID = ui->tab0_GodSon_ID->text();
    QString Name = ui->tab0_GodSon_Name->text();
    QString Surname = ui->tab0_GodSon_Surname->text();
    bool    DateAvail = ui->tab0_GodSon_DateCheckBox->isChecked();
    QString Date = ui->tab0_GodSon_Date->text();
    bool    CityAvail = ui->tab0_GodSon_CityCheckBox->isChecked();
    int     City = ui->tab0_GodSon_City->currentIndex() + 1;
    bool    NeighAvail = ui->tab0_GodSon_NeighborhoodCheckBox->isChecked();
    QString Neighborhood = ui->tab0_GodSon_Neighborhood->currentText();
    QString GP_ID = ui->tab0_GodParent_ID->text();
    QString GP_Name = ui->tab0_GodParent_Name->text();
    QString GP_Surname = ui->tab0_GodParent_Surname->text();

    //Get ID_Place
    query.prepare("SELECT ID_Place FROM Place WHERE ID_City LIKE :city AND Name LIKE :neigh ORDER BY ID_Place ASC");
    query.bindValue(":city", QVariant((CityAvail)?QString::number(City):"%"));
    query.bindValue(":neigh", QVariant((NeighAvail)?Neighborhood:"%"));
    query.exec();
    int first = 0, last;
    while(query.next())
    {
        if (first == 0)
        {
            first = query.value("ID_Place").toInt();
        }
        last = query.value("ID_Place").toInt();
    }
    //Setup Query
    QString placeholder = "%";
    query.prepare("SELECT Godsons.ID_Code AS 'Id', Godsons.FirstName AS 'Nombre', Godsons.LastName AS 'Apellido', City.Name AS 'Ciudad' , Place.Name AS 'Barrio', (GodParents.FirstName || ' ' ||GodParents.LastName) AS 'Padrino'FROM Godsons INNER JOIN Place ON Godsons.ID_Place = Place.ID_Place INNER JOIN City ON Place.ID_City = City.ID_City INNER JOIN GodParents ON Godsons.ID_GodParent = GodParents.ID_GodParent WHERE Godsons.ID_Code LIKE :id AND Godsons.FirstName LIKE :name AND Godsons.LastName LIKE :surname AND Godsons.BirthDate LIKE :bd AND Godsons.ID_Place BETWEEN :pfirst AND :plast AND GodParents.ID_GodParent LIKE :gp_id AND GodParents.FirstName LIKE :gp_name AND GodParents.LastName LIKE :gp_surname");
    query.bindValue(":id", (ID.isEmpty())?placeholder:ID.append(placeholder));
    query.bindValue(":name", (Name.isEmpty())?placeholder:Name.append(placeholder));
    query.bindValue(":surname", (Surname.isEmpty())?placeholder:Surname.append(placeholder));
    query.bindValue(":pfirst", first);
    query.bindValue(":plast", last);
    query.bindValue(":bd", (DateAvail)?Date:placeholder);
    query.bindValue(":gp_id", (GP_ID.isEmpty())?placeholder:GP_ID.append(placeholder));
    query.bindValue(":gp_name", (GP_Name.isEmpty())?placeholder:GP_Name.append(placeholder));
    query.bindValue(":gp_surname", (GP_Surname.isEmpty())?placeholder:GP_Surname.append(placeholder));

    query.exec();
    tab0_model->setQuery(query);
    ui->tab0_TableView->setModel(tab0_model);

}

void MainWindow::tab0_Query_Slot()
{
    tab0_Query();
}

void MainWindow::tab0_GodSon_ID_textChanged(const QString &arg1)
{
    QPalette *palette = new QPalette();

    if(!arg1.isEmpty())
    {

        QString workString = arg1;
        for (int i = arg1.size(); i<4; i++)
        {
            workString.append(QString::number(0));
        }

        if(!(workString[0].isLetter()) || workString[1].isLetter() || workString[2].isLetter() || workString[3].isLetter())
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->tab0_GodSon_ID->setPalette(*palette);
        }
        else
        {
            palette->setColor(QPalette::Text,Qt::black);
            ui->tab0_GodSon_ID->setPalette(*palette);
        }
    }
    else
    {
        palette->setColor(QPalette::Text,Qt::black);
        ui->tab0_GodSon_ID->setPalette(*palette);
    }

    tab0_Query();

}

void MainWindow::tab0_GodSon_DateCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->tab0_GodSon_Date->setEnabled(0);
    }
    else
    {
        ui->tab0_GodSon_Date->setEnabled(1);
    }
    tab0_Query();
}

void MainWindow::tab0_GodSon_CityCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->tab0_GodSon_City->setEnabled(0);
        ui->tab0_GodSon_NeighborhoodCheckBox->setChecked(0);
        ui->tab0_GodSon_NeighborhoodCheckBox->setEnabled(0);
    }
    else
    {
        ui->tab0_GodSon_City->setEnabled(1);
        ui->tab0_GodSon_NeighborhoodCheckBox->setEnabled(1);
    }
    tab0_Query();
}

void MainWindow::tab0_GodSon_City_currentIndexChanged(int index)
{
    index += 1;

    QSqlQuery setData;
    setData.prepare("SELECT Name FROM Place WHERE ID_City = :id");
    setData.bindValue(":id", index);
    setData.exec();
    QStringList temp = tab0_neighborhoods;
    tab0_neighborhoods.clear();
    while(setData.next())
    {
        tab0_neighborhoods.append(setData.value("Name").toString());
    }
    if (temp != tab0_neighborhoods)
    {
        ui->tab0_GodSon_Neighborhood->clear();
        ui->tab0_GodSon_Neighborhood->addItems(tab0_neighborhoods);
    }
    tab0_Query();
}

void MainWindow::tab0_GodSon_NeighborhoodCheckBox_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->tab0_GodSon_Neighborhood->setEnabled(0);
    }
    else
    {
        ui->tab0_GodSon_Neighborhood->setEnabled(1);
    }
    tab0_Query();
}

void MainWindow::tab0_GodParent_ID_textChanged(const QString &arg1)
{
    QPalette *palette = new QPalette();

    if(!arg1.isEmpty())
    {
        QString workString = arg1;
        for (int i = arg1.size(); i<4; i++)
        {
            workString.append(QString::number(0));
        }

        if(!(workString[0].isNumber() && workString[1].isNumber() && workString[2].isNumber() && workString[3].isNumber()))
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->tab0_GodParent_ID->setPalette(*palette);
        }
        else
        {
            palette->setColor(QPalette::Text,Qt::black);
            ui->tab0_GodParent_ID->setPalette(*palette);
        }
    }
    else
    {
        palette->setColor(QPalette::Text,Qt::black);
        ui->tab0_GodParent_ID->setPalette(*palette);
    }

    tab0_Query();
}

void MainWindow::tab0_Erase_clicked()
{
    ui->tab0_GodSon_ID->setText("");
    ui->tab0_GodSon_Name->setText("");
    ui->tab0_GodSon_Surname->setText("");
    ui->tab0_GodSon_DateCheckBox->setChecked(0);
    ui->tab0_GodSon_Date->setDate(QDate (2000, 01, 01));
    ui->tab0_GodSon_CityCheckBox->setChecked(0);
    ui->tab0_GodSon_City->setCurrentIndex(0);
    ui->tab0_GodSon_NeighborhoodCheckBox->setChecked(0);
    ui->tab0_GodSon_Neighborhood->setCurrentIndex(0);
    ui->tab0_GodParent_ID->setText("");
    ui->tab0_GodParent_Name->setText("");
    ui->tab0_GodParent_Surname->setText("");

}

void MainWindow::tab0_TableView_doubleClicked(const QModelIndex &index)
{
    tab0_searchIndex = ui->tab0_TableView->model()->data(ui->tab0_TableView->model()->index(index.row(), 0)).toString();
    tab1_loadData();
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::tab0_SearchOpen_clicked()
{
    int index = ui->tab0_TableView->selectionModel()->currentIndex().row();
    tab0_searchIndex = ui->tab0_TableView->model()->data(ui->tab0_TableView->model()->index(index, 0)).toString();

    if (tab0_searchIndex.isEmpty())
    {
        return;
    }
    tab1_loadData();
    ui->tabWidget->setCurrentIndex(1);
}

//--------------------------------------------------------------------------
//Tab1

void MainWindow::tab1_setEditMode(bool status)
{
    tab1_editMode = status;

    QList<QLineEdit*> lineEdit_List = ui->tab_searchView->findChildren<QLineEdit*>();
    foreach (QLineEdit* lineEdit, lineEdit_List)
    {
        lineEdit->setReadOnly(!status);
    }
    ui->tab1_BasicInfo_Age->setReadOnly(true);
    QList<QPlainTextEdit*> plainTextEdit_List = ui->tab_searchView->findChildren<QPlainTextEdit*>();
    foreach (QPlainTextEdit* plainTextEdit, plainTextEdit_List)
    {
        plainTextEdit->setReadOnly(!status);
    }
    QList<QCheckBox*> checkBox_List = ui->tab_searchView->findChildren<QCheckBox*>();
    foreach (QCheckBox* checkBox, checkBox_List)
    {
        checkBox->setEnabled(status);
    }
    QList<QDateEdit*> dateEdit_List = ui->tab_searchView->findChildren<QDateEdit*>();
    foreach (QDateEdit* dateEdit, dateEdit_List)
    {
        dateEdit->setReadOnly(!status);
        if (status)
        {
            dateEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        }
        else
        {
            dateEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
        }
    }
    QList<QComboBox*> comboBox_List = ui->tab_searchView->findChildren<QComboBox*>();
    foreach (QComboBox* comboBox, comboBox_List)
    {
        comboBox->setEnabled(status);
    }
    if (status)
    {
        ui->tab1_ProgramData_GodParent_Edit->show();
        ui->tab1_ProgramData_GodParent_Edit_Spacer->changeSize(40,20, QSizePolicy::Expanding);
        ui->tab1_saveChanges->show();
        ui->tab1_Visits_LastVisit_Date->setReadOnly(true);
        ui->tab1_Visits_LastVisit_Date->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->tab1_Visits_LastVisit_Meeting->setEnabled(false);
        ui->tab1_Visits_LastVisit_GeneralStatus->setEnabled(false);
        ui->tab1_Visits_LastVisit_Comment->setReadOnly(true);
        ui->tab1_InChargeOf_Regional->setEnabled(false);
    }
    else
    {
        ui->tab1_ProgramData_GodParent_Edit->hide();
        ui->tab1_ProgramData_GodParent_Edit_Spacer->changeSize(0,0, QSizePolicy::Maximum, QSizePolicy::Maximum);
        ui->tab1_saveChanges->hide();
    }
}

void MainWindow::tab1_basicInfo_City_currentIndexChanged(int index)
{
    index += 1;

    QSqlQuery setData;
    setData.prepare("SELECT Name FROM Place WHERE ID_City = :id");
    setData.bindValue(":id", index);
    setData.exec();
    QStringList temp = tab1_neighborhoods;
    tab1_neighborhoods.clear();
    while(setData.next())
    {
        tab1_neighborhoods.append(setData.value("Name").toString());
    }
    if (temp != tab1_neighborhoods)
    {
        ui->tab1_BasicInfo_Neighborhood->clear();
        ui->tab1_BasicInfo_Neighborhood->addItems(tab1_neighborhoods);
    }

}

void MainWindow::tab1_goBack_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::tab1_edit_clicked()
{
    if (!tab1_editMode)
    {
        ui->tab1_goBack->setVisible(false);
        ui->tab1_saveChanges->setVisible(true);
        ui->tab1_edit->setText("Cancelar");
        ui->tab1_edit->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Warning_Icon.png")));

        tab1_setEditMode(!tab1_editMode);
    }
    else
    {
        QMessageBox cancel(
                    QMessageBox::Warning,
                    "Confirmar",
                    "Realmente desea cancelar la edición e ignorar los cambios?",
                    QMessageBox::Yes | QMessageBox::No);
        cancel.setButtonText(QMessageBox::Yes, "Si");
        cancel.setButtonText(QMessageBox::No, "No");
        cancel.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Warning_Icon.png")));

        switch(cancel.exec())
        {
        case (QMessageBox::No):
            break;
        case (QMessageBox::Yes):
            tab1_setEditMode(false);
            tab1_loadData();
            ui->tab1_goBack->setVisible(true);
            ui->tab1_saveChanges->setVisible(false);
            ui->tab1_edit->setText("Editar");
            ui->tab1_edit->setIcon(QIcon(exePath.absolutePath().append("/resources/icons/Edit_Icon.png")));
            break;
        }
    }
}


void MainWindow::tab1_save_clicked()
{
    QMessageBox cancel(
                QMessageBox::Warning,
                "Confirmar",
                "Realmente desea guardar los Cambios?",
                QMessageBox::Yes | QMessageBox::No);
    cancel.setButtonText(QMessageBox::Yes, "Si");
    cancel.setButtonText(QMessageBox::No, "No");
    cancel.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Save_Icon.png")));

    switch(cancel.exec())
    {
    case (QMessageBox::No):
        break;
    case (QMessageBox::Yes):
        tab1_saveData();
        tab1_setEditMode(false);
        ui->tab1_goBack->setVisible(true);
        ui->tab1_saveChanges->setVisible(false);
        ui->tab1_edit->setText("Editar");
        tab1_loadData();
        break;
    }
}

void MainWindow::tab1_loadData()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Godsons WHERE ID_Code = :id");
    query.bindValue(":id", tab0_searchIndex);
    query.exec();
    query.next();
    if (query.value("ProgramMemberAvail").toBool())
    {
        ui->tab1_ProgramData_To_Label->hide();
        ui->tab1_ProgramData_To->hide();
        ui->tab1_ProgramData_To_Cause->hide();
        ui->tab1_ProgramData_To_Cause_Label->hide();
        ui->tab1_ProgramData_Remove->show();
        ui->tab1_ProgramData_To_Cancel->hide();
        ui->tab1_ProgramData_To_Accept->hide();
    }
    else
    {
        ui->tab1_ProgramData_StatusLabel->setText("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#ff0000;\">Dado de baja</span></p></body></html>");
        ui->tab1_ProgramData_To_Label->show();
        ui->tab1_ProgramData_To->show();
        ui->tab1_ProgramData_To->setDate(QDate::fromString(query.value("ProgramOutDate").toString(), "dd/MM/yyyy"));
        ui->tab1_ProgramData_To_Cause_Label->show();
        ui->tab1_ProgramData_To_Cause->show();
        ui->tab1_ProgramData_To_Cause->setPlainText(query.value("ProgramOutCause").toString());
        ui->tab1_ProgramData_Remove->hide();
        ui->tab1_ProgramData_To_Cancel->hide();
        ui->tab1_ProgramData_To_Accept->hide();
    }
    if(query.value("Gender").toString() == "F")
    {
        ui->tab1_ProgramData_StatusLabel->setText("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#00aa00;\">Activa</span></p></body></html>");
    }
    else
    {
        ui->tab1_ProgramData_StatusLabel->setText("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#00aa00;\">Activo</span></p></body></html>");
    }

    ui->tab1_ProgramData_Since->setDate(QDate::fromString(query.value("ProgramInDate").toString(), "dd/MM/yyyy"));
    ui->tab1_BasicInfo_ID->setText(query.value("ID_Code").toString());
    ui->tab1_BasicInfo_Name->setText(query.value("FirstName").toString());
    ui->tab1_BasicInfo_Surname->setText(query.value("LastName").toString());
    ui->tab1_BasicInfo_Gender->setCurrentText(query.value("Gender").toString());
    ui->tab1_BasicInfo_Date->setDate(QDate::fromString(query.value("BirthDate").toString(), "dd/MM/yyyy"));
    ui->tab1_BasicInfo_Age->setValue(getAge(QDate::fromString(query.value("BirthDate").toString(), "dd/MM/yyyy")));
    ui->tab1_Health_HIV->setChecked(query.value("HealthHIV").toBool());
    ui->tab1_Health_General->setCurrentIndex(query.value("HealthStatus").toInt());
    ui->tab1_Health_Comment->setPlainText(query.value("HealthComment").toString());
    ui->tab1_School_Assists->setChecked(query.value("SchoolAssists").toBool());
    ui->tab1_School_Year->setCurrentIndex(query.value("SchoolYear").toInt() - 1);
    ui->tab1_School_Distance->setCurrentIndex(query.value("SchoolDistance").toInt());
    ui->tab1_School_Comment->setPlainText(query.value("SchoolComment").toString());

    ui->tab1_Family_Mother_Name->setText(query.value("MotherName").toString());
    bool MotherDeathDateKnown = query.value("MotherDeathDateKnown").toBool();
    if (MotherDeathDateKnown)
    {
        ui->tab1_Family_Mother_DeathDate_Known->setChecked(!MotherDeathDateKnown);
        ui->tab1_Family_Mother_DeathDate->show();
        ui->tab1_Family_Mother_DeathDate->setDate(QDate::fromString(query.value("MotherDeathDate").toString(), "dd/MM/yyyy"));
    }
    else
    {
        ui->tab1_Family_Mother_DeathDate_Known->setChecked(!MotherDeathDateKnown);
        ui->tab1_Family_Mother_DeathDate->hide();
    }
    ui->tab1_Family_Mother_DeathCause->setPlainText(query.value("MotherCOD").toString());

    ui->tab1_Family_Father_Name->setText(query.value("FatherName").toString());
    bool FatherDeathDateKnown = query.value("FatherDeathDateKnown").toBool();
    if (FatherDeathDateKnown)
    {
        ui->tab1_Family_Father_DeathDate_Known->setChecked(!FatherDeathDateKnown);
        ui->tab1_Family_Father_DeathDate->show();
        ui->tab1_Family_Father_DeathDate->setDate(QDate::fromString(query.value("FatherDeathDate").toString(), "dd/MM/yyyy"));
    }
    else
    {
        ui->tab1_Family_Father_DeathDate_Known->setChecked(!FatherDeathDateKnown);
        ui->tab1_Family_Father_DeathDate->hide();
    }
    ui->tab1_Family_Father_DeathCause->setPlainText(query.value("FatherCOD").toString());

    ui->tab1_Family_OtherRelated->setPlainText(query.value("OtherRelatedFamiliars").toString());
    ui->tab1_History_Text->setPlainText(query.value("History").toString());
    ui->tab1_Comment_Text->setPlainText(query.value("Comment").toString());

    QSqlQuery query2;
    query2.prepare("SELECT Place.ID_City AS 'City', Place.Name AS 'Neigborhood' FROM Place WHERE Place.ID_Place = :place");
    query2.bindValue(":place", query.value("ID_Place").toInt());
    query2.exec();
    query2.next();
    ui->tab1_BasicInfo_City->setCurrentIndex(query2.value("City").toInt() - 1);
    ui->tab1_BasicInfo_Neighborhood->setCurrentText(query2.value("Neigborhood").toString());

    query2.clear();
    query2.prepare("SELECT GodParents.FirstName, GodParents.LastName FROM GodParents WHERE GodParents.ID_GodParent = :gp");
    query2.bindValue(":gp", query.value("ID_GodParent").toInt());
    query2.exec();
    query2.next();
    ui->tab1_ProgramData_GodParent_ID->setText(query.value("ID_GodParent").toString());
    ui->tab1_ProgramData_GodParent_Name->setText(query2.value("FirstName").toString());
    ui->tab1_ProgramData_GodParent_Surname->setText(query2.value("LastName").toString());

    query2.clear();
    tab1_ICOid = query.value("ID_InChargeOf").toInt();
    query2.prepare("SELECT * FROM InChargeOf WHERE ID_InChargeOf = :ico");
    query2.bindValue(":ico", query.value("ID_InChargeOf").toInt());
    query2.exec();
    query2.next();
    ui->tab1_InChargeOf_Name->setText(query2.value("Name").toString());
    int age = query2.value("AgeWhenCreated").toInt();
    QDate DateCreated = QDate::fromString(query2.value("DateCreated").toString(), "dd/MM/yyyy");
    age += QDate::currentDate().year() - DateCreated.year();
    ui->tab1_InChargeOf_Age->setValue(age);
    tab1_ICOage = age;
    ui->tab1_InChargeOf_Relationship->setText(query2.value("Relationship").toString());
    tab1_isRegional = query2.value("RegionalGroup").toBool();
    ui->tab1_InChargeOf_Regional->setChecked(tab1_isRegional);
    ui->tab1_InChargeOf_Comment->setPlainText(query2.value("Comment").toString());

    query2.clear();
    query2.prepare("SELECT Visits.Date AS 'Fecha', CASE (Visits.Meeting) WHEN 1 THEN 'Si'ELSE 'No' END AS 'Encuentro?', CASE (Visits.GeneralStatus) WHEN 0 THEN 'Bueno' WHEN 1 THEN 'Regular' WHEN 2 THEN 'Malo' ELSE 'Muy Malo' END AS 'Estado General' FROM Visits WHERE Visits.ID_GodsonCode = :id ORDER BY Visits.Date DESC");
    query2.bindValue(":id", tab0_searchIndex);
    query2.exec();
    tab1_model = new QSqlQueryModel;
    tab1_model->setQuery(query2);
    ui->tab1_Visits_Table->setModel(tab1_model);

    query2.clear();

    query2.prepare("SELECT Visits.Date, Visits.Meeting, Visits.GeneralStatus, Visits.Comment FROM Visits WHERE Visits.ID_GodsonCode = :id ORDER BY Visits.Date DESC LIMIT 1");
    query2.bindValue(":id", tab0_searchIndex);
    query2.exec();
    query2.next();
    ui->tab1_Visits_LastVisit_Date->setDate(QDate::fromString(query2.value("Date").toString(),"dd/MM/yyyy"));
    ui->tab1_Visits_LastVisit_Meeting->setChecked(query2.value("Meeting").toBool());
    ui->tab1_Visits_LastVisit_GeneralStatus->setCurrentIndex(query2.value("GeneralStatus").toInt());
    ui->tab1_Visits_LastVisit_Comment->setPlainText(query2.value("Comment").toString());
}

void MainWindow::tab1_saveData()
{
    QSqlQuery query;
    QSqlQuery query2;
    query.prepare("UPDATE Godsons SET FirstName = :name, LastName = :surname, BirthDate = :birthdate, Gender = :gender, ID_InChargeOf = :idinchargeof, SchoolAssists = :schoolassists, SchoolDistance = :schooldistance, SchoolYear = :schoolyear, SchoolComment = :schoolcomment, HealthHIV = :healthhiv, HealthStatus = :healthstatus, HealthComment = :healthcomment, ID_Place = :idplace, MotherName = :mothername, MotherCOD = :mothercod, MotherDeathDateKnown = :motherdeathdateknown, MotherDeathDate = :motherdeathdate, FatherName = :fathername, FatherCOD = :fathercod, FatherDeathDateKnown = :fatherdeathdateknown, FatherDeathDate = :fatherdeathdate, OtherRelatedFamiliars = :otherrelatedfamiliars, History = :history, Comment = :comment WHERE ID_Code = :id");
    query.bindValue(":name", ui->tab1_BasicInfo_Name->text());
    query.bindValue(":surname", ui->tab1_BasicInfo_Surname->text());
    query.bindValue(":birthdate", ui->tab1_BasicInfo_Date->date().toString("dd/MM/yyyy"));
    query.bindValue(":gender", ui->tab1_BasicInfo_Gender->currentText());
    query.bindValue(":schoolassists", ui->tab1_School_Assists->isChecked());
    query.bindValue(":schooldistance", ui->tab1_School_Distance->currentIndex());
    query.bindValue(":schoolyear", ui->tab1_School_Year->currentIndex() + 1);
    query.bindValue(":schoolcomment", ui->tab1_School_Comment->toPlainText());
    query.bindValue(":healthhiv", ui->tab1_Health_HIV->isChecked());
    query.bindValue(":healthstatus", ui->tab1_Health_General->currentIndex());
    query.bindValue(":healthcomment", ui->tab1_Health_Comment->toPlainText());

    //Get ID_Place
    query2.prepare("SELECT ID_Place FROM Place WHERE ID_City = :city AND Name = :neigh");
    query2.bindValue(":city", ui->tab1_BasicInfo_City->currentIndex() + 1);
    query2.bindValue(":neigh", ui->tab1_BasicInfo_Neighborhood->currentText());
    query2.exec();
    query2.next();
    int idPlace = query2.value("ID_Place").toInt();
    query.bindValue(":idplace", idPlace);
    query2.clear();

    query.bindValue(":mothername", ui->tab1_Family_Mother_Name->text());
    query.bindValue(":mothercod", ui->tab1_Family_Mother_DeathCause->toPlainText());
    query.bindValue(":motherdeathdateknown", !ui->tab1_Family_Mother_DeathDate_Known->isChecked());
    query.bindValue(":motherdeathdate", ui->tab1_Family_Mother_DeathDate->date().toString("dd/MM/yyyy"));

    query.bindValue(":fathername", ui->tab1_Family_Father_Name->text());
    query.bindValue(":fathercod", ui->tab1_Family_Father_DeathCause->toPlainText());
    query.bindValue(":fatherdeathdateknown", !ui->tab1_Family_Father_DeathDate_Known->isChecked());
    query.bindValue(":fatherdeathdate", ui->tab1_Family_Father_DeathDate->date().toString("dd/MM/yyyy"));

    query.bindValue(":otherrelatedfamiliars", ui->tab1_Family_OtherRelated->toPlainText());
    query.bindValue(":history", ui->tab1_History_Text->toPlainText());
    query.bindValue(":comment", ui->tab1_Comment_Text->toPlainText());

    int ICOidTemp;
    if (!(ui->tab1_InChargeOf_Regional->isChecked()) && !tab1_isRegional)
    {
        query2.prepare("UPDATE InChargeOf SET Name = :name, Relationship = :rel, Comment = :comment WHERE ID_InChargeOf = :id");
        query2.bindValue(":name", ui->tab1_InChargeOf_Name->text());
        query2.bindValue(":rel", ui->tab1_InChargeOf_Relationship->text());
        query2.bindValue(":comment", ui->tab1_InChargeOf_Comment->toPlainText());
        query2.bindValue(":id", tab1_ICOid);
        query2.exec();
        if (tab1_ICOage != ui->tab1_InChargeOf_Age->value())
        {
            query2.clear();
            query2.prepare("UPDATE InChargeOf SET AgeWhenCreated = :age, DateCreated = :date WHERE ID_InChargeOf = :id");
            query2.bindValue(":age", ui->tab1_InChargeOf_Age->value());
            query2.bindValue(":date", QDate::currentDate().toString("dd/MM/yyyy"));
            query2.bindValue(":id", tab1_ICOid);
            query2.exec();
        }
        query.bindValue(":idinchargeof", tab1_ICOid);
    }
    else if(ui->tab1_InChargeOf_Regional->isChecked() && !tab1_isRegional)
    {
        query.bindValue(":idinchargeof", tab1_NewICOid);
    }
    else if (!(ui->tab1_InChargeOf_Regional->isChecked()) && tab1_isRegional)
    {
        query2.clear();
        query2.prepare("INSERT INTO InChargeOf(Name, AgeWhenCreated, Relationship, RegionalGroup, DateCreated, Comment) VALUES(:name, :age, :rel, :reg, :date, :comment)");
        query2.bindValue(":name", ui->tab1_InChargeOf_Name->text());
        query2.bindValue(":age", ui->tab1_InChargeOf_Age->value());
        query2.bindValue(":rel", ui->tab1_InChargeOf_Relationship->text());
        query2.bindValue(":reg", 0);
        query2.bindValue(":date", QDate::currentDate().toString("dd/MM/yyyy"));
        query2.bindValue(":comment", ui->tab1_InChargeOf_Comment->toPlainText());
        query2.exec();

        query2.clear();
        query2.exec("SELECT InChargeOf.ID_InChargeOf FROM InChargeOf ORDER BY InChargeOf.ID_InChargeOf DESC LIMIT 1");
        query2.next();
        ICOidTemp = query2.value("ID_InChargeOf").toInt();
        query.bindValue(":idinchargeof", ICOidTemp);
    }
    else
    {
        query.bindValue(":idinchargeof", tab1_ICOid);
    }

    query.bindValue(":id", tab0_searchIndex);
    query.exec();
}

int MainWindow::getAge(QDate date)
{
    int ageReturn;
    int dateYear = date.year();
    int dateMonth = date.month();
    int dateDay = date.day();

    int currYear = QDate::currentDate().year();
    int currMonth = QDate::currentDate().month();
    int currDay = QDate::currentDate().day();

    ageReturn = currYear - dateYear;
    if (currMonth < dateMonth || (currMonth == dateMonth && currDay < dateDay))
    {
        ageReturn--;
    }
    return ageReturn;
}

void MainWindow::tab1_fatherDate(bool state)
{
    if(tab1_editMode)
    {
        if(state)
        {
            ui->tab1_Family_Father_DeathDate->hide();
        }
        else
        {
            ui->tab1_Family_Father_DeathDate->show();
        }
    }
}

void MainWindow::tab1_motherDate(bool state)
{
    if(tab1_editMode)
    {
        if(state)
        {
            ui->tab1_Family_Mother_DeathDate->hide();
        }
        else
        {
            ui->tab1_Family_Mother_DeathDate->show();
        }
    }
}

void MainWindow::tab1_ProgramData_Remove()
{
    ui->tab1_ProgramData_StatusLabel->setText("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#ff0000;\">Dado de baja</span></p></body></html>");
    ui->tab1_ProgramData_To_Label->show();
    ui->tab1_ProgramData_To->show();
    ui->tab1_ProgramData_To_Cause_Label->show();
    ui->tab1_ProgramData_To_Cause->show();
    ui->tab1_ProgramData_Remove->hide();
    ui->tab1_ProgramData_To_Cancel->show();
    ui->tab1_ProgramData_To_Accept->show();

}

void MainWindow::tab1_ProgramData_Cancel()
{
    tab1_loadData();
}

void MainWindow::tab1_ProgramData_Accept()
{
    QMessageBox cancel(
                QMessageBox::Warning,
                "Confirmar",
                "Realmente desea dar de Baja al ahijado?",
                QMessageBox::Yes | QMessageBox::No);
    cancel.setButtonText(QMessageBox::Yes, "Si");
    cancel.setButtonText(QMessageBox::No, "No");
    cancel.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Save_Icon.png")));

    switch(cancel.exec())
    {
    case (QMessageBox::No):
        tab1_loadData();
        break;
    case (QMessageBox::Yes):
        QSqlQuery query;
        query.prepare("UPDATE Godsons SET ProgramMemberAvail = 0, ProgramOutDate = :out, ProgramOutCause = :cause WHERE ID_Code = :id");
        query.bindValue(":out", QDate::currentDate().toString("dd/MM/yyyy"));
        query.bindValue(":cause", ui->tab1_ProgramData_To_Cause->toPlainText());
        query.bindValue(":id", tab0_searchIndex);
        query.exec();
        ui->tab1_ProgramData_To_Accept->hide();
        ui->tab1_ProgramData_To_Cancel->hide();
        break;
    }

}
//--------------------------------------------------------------------------
//Tab2
void MainWindow::tab2_Next_clicked()
{

}

//--------------------------------------------------------------------------
//Tab5

void MainWindow::tab5_erase()
{
    QSqlQuery query;
    query.exec("SELECT GodParents.ID_GodParent FROM GodParents ORDER BY GodParents.ID_GodParent DESC LIMIT 1");
    query.next();
    ui->tab5_ID->setText(QString::number(query.value("ID_GodParent").toInt() + 1));
    ui->tab5_Name->setText("");
    ui->tab5_Surname->setText("");
    ui->tab5_Email->setText("");
    ui->tab5_Tel->setText("");
    ui->tab5_Cel->setText("");
    ui->tab5_Country->setText("Argentina");
    ui->tab5_State->setText("Buenos Aires");
    ui->tab5_City->setText("CABA");
    ui->tab5_Address->setText("");
    ui->tab5_Zip->setText("");
    ui->tab5_Comment->setPlainText("");
}

void MainWindow::tab5_Erase_clicked()
{
    tab5_erase();
}

void MainWindow::tab5_Create_clicked()
{
    if (ui->tab5_Address->text() != "" && ui->tab5_City->text() != "" && ui->tab5_Country->text() != "" &&
            ui->tab5_Name->text() != "" && ui->tab5_State->text() != "" && ui->tab5_Surname->text() != "" &&
            (ui->tab5_Cel->text() != "" || ui->tab5_Email->text() != "" || ui->tab5_Tel->text() != ""))
    {
        QSqlQuery query;
        query.prepare("INSERT INTO GodParents(FirstName, LastName, Email, Telephone, CellPhone, Country, State, City, Address, ZipCode, SinceDate, Comment) VALUES (:name, :surname, :email, :tel, :cel, :country, :state, :city, :address, :zip, :date, :comment)");
        query.bindValue(":name", ui->tab5_Name->text());
        query.bindValue(":surname", ui->tab5_Surname->text());
        query.bindValue(":email", ui->tab5_Email->text());
        query.bindValue(":tel", ui->tab5_Tel->text());
        query.bindValue(":country", ui->tab5_Country->text());
        query.bindValue(":cel", ui->tab5_Cel->text());
        query.bindValue(":state", ui->tab5_State->text());
        query.bindValue(":city", ui->tab5_City->text());
        query.bindValue(":address", ui->tab5_Address->text());
        query.bindValue(":zip", ui->tab5_Zip->text());
        query.bindValue(":date", QDate::currentDate().toString("dd/MM/yyyy"));
        query.bindValue(":comment", ui->tab5_Comment->toPlainText());
        if (query.exec())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Operación completada");
            msgBox.setInformativeText("Se ha agregado al nuevo padrino a la base de datos");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Ok_Icon.png")));
            msgBox.exec();

            tab5_erase();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Operación fallida");
            msgBox.setInformativeText("Ha habido un problema la ejecutar el ingreso de la información a la base de datos");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Warning_Icon.png")));
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setInformativeText("Uno o mas de los campos requeridos se encuentran en blanco");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setWindowIcon(QIcon(exePath.absolutePath().append("/resources/icons/Warning_Icon.png")));
        msgBox.exec();
    }
}

//--------------------------------------------------------------------------
//Tab6

void MainWindow::tab6_Query()
{
    tab6_model = new QSqlQueryModel;
    QSqlQuery query;
    QString ID = ui->tab6_GodParent_ID->text();
    QString Name = ui->tab6_GodParent_Name->text();
    QString Surname = ui->tab6_GodParent_Surname->text();

    QString placeholder = "%";
    query.prepare("SELECT GodParents.ID_GodParent AS 'ID', GodParents.FirstName AS 'Nombre', GodParents.LastName AS 'Apellido', GodParents.GodSons_Number AS 'Ahijados' FROM GodParents WHERE GodParents.ID_GodParent LIKE :id AND GodParents.FirstName LIKE :name AND GodParents.LastName LIKE :surname");
    query.bindValue(":id", (ID.isEmpty())?placeholder:ID.append(placeholder));
    query.bindValue(":name", (Name.isEmpty())?placeholder:Name.append(placeholder));
    query.bindValue(":surname", (Surname.isEmpty())?placeholder:Surname.append(placeholder));

    query.exec();
    tab6_model->setQuery(query);
    ui->tab6_tableView->setModel(tab6_model);
}

void MainWindow::tab6_Query_Slot()
{
    tab6_Query();
}

void MainWindow::tab6_GodParent_ID_textChanged(const QString &arg1)
{
    QPalette *palette = new QPalette();

    if(!arg1.isEmpty())
    {
        QString workString = arg1;
        for (int i = arg1.size(); i<4; i++)
        {
            workString.append(QString::number(0));
        }

        if(!(workString[0].isNumber() && workString[1].isNumber() && workString[2].isNumber() && workString[3].isNumber()))
        {
            palette->setColor(QPalette::Text,Qt::red);
            ui->tab6_GodParent_ID->setPalette(*palette);
        }
        else
        {
            palette->setColor(QPalette::Text,Qt::black);
            ui->tab6_GodParent_ID->setPalette(*palette);
        }
    }
    else
    {
        palette->setColor(QPalette::Text,Qt::black);
        ui->tab6_GodParent_ID->setPalette(*palette);
    }

    tab6_Query();
}

void MainWindow::tab6_Erase_clicked()
{
    ui->tab6_GodParent_ID->setText("");
    ui->tab6_GodParent_Name->setText("");
    ui->tab6_GodParent_Surname->setText("");
}

void MainWindow::tab6_SearchOpen_clicked()
{
    int index = ui->tab6_tableView->selectionModel()->currentIndex().row();
    tab6_searchIndex = ui->tab6_tableView->model()->data(ui->tab6_tableView->model()->index(index, 0)).toString();

    if (tab6_searchIndex.isEmpty())
    {
        return;
    }
    tab7_loadData();
    ui->tabWidget->setCurrentIndex(7);
}

void MainWindow::tab6_TableView_doubleClicked(const QModelIndex &index)
{
    tab6_searchIndex = ui->tab6_tableView->model()->data(ui->tab6_tableView->model()->index(index.row(), 0)).toString();
    tab7_loadData();
    ui->tabWidget->setCurrentIndex(7);
}

//--------------------------------------------------------------------------
//Tab7

void MainWindow::tab7_loadData()
{
    tab7_model = new QSqlQueryModel;
    QSqlQuery query;

    query.prepare("SELECT * FROM GodParents WHERE GodParents.ID_GodParent = :id");
    qDebug() <<tab6_searchIndex;
    query.bindValue(":id", tab6_searchIndex);
    query.exec();
    query.next();

    int id = query.value("ID_GodParent").toInt();
    ui->tab7_ID->setText(query.value("ID_GodParent").toString());
    ui->tab7_Name->setText(query.value("FirstName").toString());
    ui->tab7_Surname->setText(query.value("LastName").toString());
    ui->tab7_Email->setText(query.value("Email").toString());
    ui->tab7_Tel->setText(query.value("Telephone").toString());
    ui->tab7_Cel->setText(query.value("CellPhone").toString());
    ui->tab7_Country->setText(query.value("Country").toString());
    ui->tab7_State->setText(query.value("State").toString());
    ui->tab7_City->setText(query.value("City").toString());
    ui->tab7_Address->setText(query.value("Address").toString());
    ui->tab7_Zip->setText(query.value("ZipCode").toString());
    ui->tab7_Date->setDate(QDate::fromString(query.value("SinceDate").toString(), "dd/MM/yyyy"));
    ui->tab7_Comment->setPlainText(query.value("Comment").toString());

    query.clear();
    query.prepare("SELECT Godsons.ID_Code AS 'Id', Godsons.FirstName AS 'Nombre', Godsons.LastName AS 'Apellido', City.Name AS 'Ciudad', Place.Name AS 'Barrio' FROM Godsons INNER JOIN Place ON Godsons.ID_Place = Place.ID_Place INNER JOIN City ON Place.ID_City = City.ID_City WHERE Godsons.ID_GodParent LIKE :gp_id");
    query.bindValue(":gp_id", id);
    query.exec();
    tab7_model->setQuery(query);
    ui->tab7_Godsons_Table->setModel(tab7_model);
}

void MainWindow::tab7_goBack_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}
