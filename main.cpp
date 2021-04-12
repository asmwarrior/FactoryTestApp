#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    a.setWindowIcon(QIcon(":/icons/application"));
    a.setOrganizationName("Capelon AB");
    a.setApplicationName("CapelonTestStation");

    MainWindow w;
    w.setWindowTitle("Capelon Test Station 0.6.31");
    w.setFixedSize(1200, 900);
    w.show();

    return a.exec();
}
