#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>

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
    mainLayout->addStretch();

    //Start testing button
    _startFullCycleTesting = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTesting->setFixedSize(180, 40);
    mainLayout->addWidget(_startFullCycleTesting);
    connect(_startFullCycleTesting, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _logWidget = new QListWidget(this);
    mainLayout->addWidget(_logWidget);

    _db = new DataBase(this);
    _db->connectToDataBase();
    _db->insertIntoTable("test", QByteArray("1234567"));
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

void MainWindow::initDali(RailtestClient *rail)
{
    rail->syncCommand("dlpw", "1", 1000);
    rail->syncCommand("dali", "0xFE80 16 0 0", 1000);
    rail->syncCommand("dali", "0xFF90 16 0 1000000", 2000);
}

void MainWindow::testRadio(RailtestClient *rail)
{
    logInfo("Testing Radio Interface...");

    _settings->beginGroup("Radio");

    RailtestClient rf;

    connect(&rf, &RailtestClient::replyReceived, this, &MainWindow::onRfReplyReceived);
    if (!rf.open(_settings->value("Serial", "COM2").toString()))
    {
        logError("Cannot open serial port for reference radio module!");
    }

    rf.syncCommand("reset", "", 3000);
    if (!rf.waitCommandPrompt())
    {
        logError("Timeout waiting reference radio module command prompt!");
    }

    rf.syncCommand("rx", "0", 1000);
    _rfRSSI = 255;
    _rfCount = 0;
    rf.syncCommand("setBleMode", "1", 1000);
    rf.syncCommand("setBle1Mbps", "1", 1000);
    rf.syncCommand("setChannel", "19", 1000);
    rail->syncCommand("rx", "0", 1000);
    rail->syncCommand("setBleMode", "1", 1000);
    rail->syncCommand("setBle1Mbps", "1", 1000);
    rail->syncCommand("setChannel", "19", 1000);
    rail->syncCommand("setPower", "80", 1000);
    rf.syncCommand("rx", "1", 1000);
    rail->syncCommand("tx", "11", 5000);

    if (_rfCount < 8)
    {
        logError(QString("Radio Interface failure: packet lost (%1)!").arg(_rfCount));
    }

    if (_rfRSSI < _settings->value("Min", -50).toInt() || _rfRSSI > _settings->value("Max", 20).toInt())
    {
        logError(QString("Radio Interface failure: RSSI (%1) is out of bounds!").arg(_rfRSSI));
    }

    logInfo(QString("Radio Interface: RSSI=%1.").arg(_rfRSSI));
    logInfo("Radio Interface is OK.");
}

void MainWindow::testAccelerometer(RailtestClient *rail)
{
    logInfo("Testing Accelerometer...");

    auto reply = rail->syncCommand("accl");

    if (reply.isEmpty())
    {
        logError("No reply to accelerometer command!");
        return;
    }

    auto map = reply[0].toMap();

    if (map.contains("error"))
    {
        logError(QString("Accelerometer error: %1, %2!").arg(map["error"].toString()).arg(map["errorCode"].toString()));
    }

    if (map.contains("X") && map.contains("Y") && map.contains("Z"))
    {
        auto
           x = map["X"].toDouble(),
           y = map["Y"].toDouble(),
           z = map["Z"].toDouble();

        if (x > 10 || x < -10 || y > 10 || y < -10 || z < 80 || z > 100)
        {
            logError(QString("Accelerometer failure: X=%1, Y=%2, Z=%3.").arg(x).arg(y).arg(z));
        }
        else
            logInfo(QString("Accelerometer: X=%1, Y=%2, Z=%3.").arg(x).arg(y).arg(z));
    }
    else
        logError("Wrong reply to accelerometer command!");

    logInfo("Accelerometer is OK.");
}

void MainWindow::testLightSensor(RailtestClient *rail)
{
    logInfo("Testing Light Sensor...");
    rail->syncCommand("lsen");
    QThread::sleep(1);

    auto reply = rail->syncCommand("lsen");

    if (reply.isEmpty())
    {
        logError("No reply to light sensor command!");
        return;
    }

    auto map = reply[0].toMap();

    if (map.contains("error"))
    {
        logError(QString("Light Sensor error: %1, %2!").arg(map["error"].toString()).arg(map["errorCode"].toString()));
    }

    if (map.contains("opwr"))
    {
        auto opwr = map["opwr"].toDouble();

        if (opwr < 0)
        {
            logError(QString("Light Sensor failure: opwr=%1.").arg(opwr));
        }
        else
        {
            logInfo(QString("Light Sensor: opwr=%1.").arg(opwr));
        }
    }
    else
    {
        logError("Wrong reply to light sensor command!");
    }

    logInfo("Light Sensor is OK.");
}

void MainWindow::testDALI(RailtestClient *rail)
{
    logInfo("Testing DALI...");

    auto reply = rail->syncCommand("dali", "0xFF90 16 0 1000000");

    if (reply.isEmpty())
    {
        logError("No reply to DALI status command!");
        return;
    }

    auto map = reply[0].toMap();

    if (map.contains("error") && map.contains("reply_bits") && map.contains("reply_data"))
    {
        auto
            error = map["error"].toString(),
            bits = map["reply_bits"].toString(),
            data = map["reply_data"].toString();

        if (error != "0" || bits != "8")
        {
            logError(QString("DALI failure: code=%1, reply_bits=%2, reply_data=%3!")
                            .arg(error).arg(bits).arg(data));
        }
        else
            logInfo(QString("DALI status: reply_data=%1.").arg(data));
    }
    else
        if (map.contains("error"))
        {
            logError(QString("DALI error: %1, %2!")
                            .arg(map["error"].toString()).arg(map["errorCode"].toString()));
        }
        else
        {
            logError("Wrong reply to DALI status command!");
        }

    rail->syncCommand("dali", "0xFE00 16 0 0");
    rail->syncCommand("dlpw", "0");
    logInfo("DALI is OK.");
}

void MainWindow::testGNSS(RailtestClient *rail)
{
    logInfo("Testing GNSS...");

    auto replies = rail->syncCommand("gnrx", "3", 15000);

    if (replies.isEmpty())
    {
        logError("No reply to GNSS command!");
        return;
    }

    foreach (auto reply, replies)
    {
        auto map = reply.toMap();

        if (map.contains("error"))
        {
            logError(QString("GNSS error: %1, %2!")
                            .arg(map["error"].toString()).arg(map["errorCode"].toString()));
        }

        if (map.contains("line"))
        {
            logInfo("GNSS reply: " + map["line"].toString());
        }
        else
        {
            logError("Wrong reply to GNSS command!");
        }
    }

    logInfo("GNSS is OK.");
}

void MainWindow::startFullCycleTesting()
{
    RailtestClient rail;

    if (!rail.open(_settings->value("Railtest/Serial", "COM1").toString()))
    {
        logError("Cannot open RAILTEST serial port!");
    }

    if (!rail.waitCommandPrompt())
    {
        logError("Timeout waiting RAILTEST command prompt!");
    }

    auto id = rail.readChipId();

    if (id.isEmpty())
    {
        logError("Cannot read unique device identifier!");
    }

    logInfo("Device ID: " + id);
    initDali(&rail);
    testRadio(&rail);
    testAccelerometer(&rail);
    testLightSensor(&rail);
    testDALI(&rail);
    testGNSS(&rail);
    logInfo("Test sequence completed.");

    downloadRailtest();
}

void MainWindow::onRfReplyReceived(QString id, QVariantMap params)
{
    if (id == "rxPacket" && params.contains("rssi"))
    {
        bool ok;
        int rssi = params.value("rssi").toInt(&ok);

        if (ok)
        {
            ++_rfCount;
            if (rssi < _rfRSSI)
                _rfRSSI = rssi;
        }
    }
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

    item->setBackground(Qt::darkRed);
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
