#include <QApplication>
#include "Breakout.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Breakout window;

    window.setWindowTitle("Window");
    window.setWindowIcon(QIcon(":/images/icon.jpg"));
    window.show();

    return a.exec();
}
