#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "ConsoleProcess.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _settings = QSharedPointer<QSettings>::create(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/settings.ini", QSettings::IniFormat);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    //Choose device type
    QLabel* selectDeviceBoxLabel = new QLabel("Step 1. Select device model", this);
    _selectDeviceModelBox = new QComboBox(this);
    _selectDeviceModelBox->setFixedWidth(150);
    _selectDeviceModelBox->addItems(_settings->value("Devices").toString().split("|"));
    connect(_selectDeviceModelBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onSelectDeviceBoxCurrentTextChanged(const QString&)));
    QHBoxLayout* selectDeviceLayout = new QHBoxLayout;
    selectDeviceLayout->addWidget(selectDeviceBoxLabel);
    selectDeviceLayout->addWidget(_selectDeviceModelBox);
    selectDeviceLayout->addStretch();

    mainLayout->addLayout(selectDeviceLayout);


    //Start testing
    _startFullCycleTesting = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTesting->setFixedSize(180, 40);
    mainLayout->addWidget(_startFullCycleTesting);
    connect(_startFullCycleTesting, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    mainLayout->addStretch();

    _logWidget = new QListWidget(this);
    mainLayout->addWidget(_logWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onSelectDeviceBoxCurrentTextChanged(const QString &text)
{
    qDebug() << text << "device selected";
}

void MainWindow::downloadRailtest()
{
    jLinkScript(_settings->value("Zhaga/Script", "olc_zhaga_software.jlink").toString());
}

void MainWindow::testRadio(RailtestClient *rail)
{

}

void MainWindow::startFullCycleTesting()
{
    RailtestClient rail;

    if (!rail.open(_settings->value("Railtest/Serial", "COM1").toString()))
    {
        logError("Cannot open RAILTEST serial port!");
    }
        //throw InputOutputError("Cannot open RAILTEST serial port!");

    if (!rail.waitCommandPrompt())
    {
    }
        //throw TestError("Timeout waiting RAILTEST command prompt!");

    auto id = rail.readChipId();

    if (id.isEmpty())
    {

    }
        //throw TestError("Cannot read unique device identifier!");

    logInfo("Device ID: " + id);
    //initDali(&rail);
    testRadio(&rail);
    //testAccelerometer(&rail);
    //testLightSensor(&rail);
   // testDALI(&rail);
    //testGNSS(&rail);
    logInfo("Test sequence completed.");

    downloadRailtest();
}

void MainWindow::logInfo(const QString &message)
{
    _logWidget->addItem(message);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void MainWindow::logError(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::red);
    item->setForeground(Qt::white);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qCritical().noquote() << message;
}

void MainWindow::logSuccess(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::green);
    item->setForeground(Qt::black);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void MainWindow::jLinkScript(const QString &fileName)
{
    ConsoleProcess jlink;
    QStringList args;

    _settings->beginGroup("JLink");
    args = _settings->value("Options").toString().split("::", QString::SkipEmptyParts);
    args.append("-CommanderScript");
    args.append(fileName);
    logInfo("JLink Commander script " + fileName + "...");
    if (!jlink.start(_settings->value("Commander", "JLink.exe").toString(), args))
    {
        logError("Cannot start JLink Commander!");
    }
        //throw InputOutputError("Cannot start JLink Commander!");

    if (!jlink.skipUntilFinished())
    {
    }
        //throw InputOutputError("JLink Commander was executed too long!");

    if (jlink.exitCode())
    {
    }
        //throw TestError("Error while executing JLink Commander script!");

    logInfo("JLink Commander script completed.");
}
