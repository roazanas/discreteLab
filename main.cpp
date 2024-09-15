#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p = qApp->palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(63,63,63));
    p.setColor(QPalette::Disabled, QPalette::Button, QColor(43,43,43));
    p.setColor(QPalette::Highlight, QColor(111,125,242));
    p.setColor(QPalette::Disabled, QPalette::Highlight, QColor(43,43,43));
    p.setColor(QPalette::AlternateBase, QColor(43,43,43));
    qApp->setPalette(p);
    w.show();
    return a.exec();
}
