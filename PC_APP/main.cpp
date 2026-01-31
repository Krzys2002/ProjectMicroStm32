#include <QApplication>
#include "Inc/MainWindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(520, 720);
    w.show();
    return app.exec();
}