#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef QT_DEBUG
    QFile styleSheet(exePath->absolutePath().append("/../../../GS/resources/stylesheets/styleSheet.qss"));
    #else
    QFile styleSheet(exePath->absolutePath().append("/resources/stylesheets/styleSheet.qss"));
    #endif

    if(styleSheet.open(QIODevice::ReadOnly | QIODevice::Text))
    {
           a.setStyleSheet(styleSheet.readAll());
           styleSheet.close();
    }
    else
    {
        QMessageBox m(QMessageBox::Critical,
                      "Error", "Hoja de estilo no encontrada:\n\nError 0x4");
        m.show();
        a.exit(4);
        //return 4;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
