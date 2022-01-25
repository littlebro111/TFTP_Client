#include "mainwindow.h"
extern FILE* LogFile;
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("TFTP Client");
    w.setFixedSize(600, 400);
    return a.exec();
}
