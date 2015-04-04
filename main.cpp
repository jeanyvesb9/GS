#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleSheet(exePath->absolutePath().append("/resources/stylesheets/styleSheet.qss"));
    if(styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
       {
           a.setStyleSheet(styleSheet.readAll());
           styleSheet.close();
       }

    MainWindow w;
    w.show();

    return a.exec();
}
