#include <QApplication>
#include "Breakout.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Breakout arkanoid;
    arkanoid.show();

    return a.exec();
}
