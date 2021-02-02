#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/application"));
    a.setOrganizationName("Capelon");
    a.setApplicationName("FactoryTestApp");

    MainWindow w;
    w.setFixedSize(800, 600);
    w.show();

    return a.exec();
}
