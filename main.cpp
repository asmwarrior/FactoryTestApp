#include "MainWindow.h"
#include "version.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("Capelon AB");
    a.setApplicationName("CapelonTestStation");
    a.setApplicationVersion(CTS_VERSION);

    MainWindow w;
    w.setWindowTitle("Capelon Test Station " + a.applicationVersion());
    w.setMinimumWidth(1200);
    w.setMinimumHeight(900);
    w.show();

    return a.exec();
}
