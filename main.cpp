#include "mainwindow.h"

#include <QApplication>
#include <QScrollArea>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QScrollArea scrollArea;
    scrollArea.setWidget(&w);
    scrollArea.show();
    // w.show();
    return a.exec();
}
