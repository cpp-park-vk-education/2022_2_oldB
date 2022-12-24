

#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    a.setWindowIcon(QIcon(":/img/img/speechbubble.png"));
    w.setWindowTitle("");

    w.show();
    return a.exec();
}
