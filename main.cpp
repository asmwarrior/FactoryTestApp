#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    a.setWindowIcon(QIcon(":/icons/application"));
    a.setOrganizationName("Capelon");
    a.setApplicationName("CapelonTestStation");

    MainWindow w;
    w.setFixedSize(1200, 900);
    w.show();

    return a.exec();
}
