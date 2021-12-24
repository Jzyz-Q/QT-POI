#include "mainwindow.h"
#include "OpenFile.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Welcome");
    //w.show();
    return a.exec();
}
