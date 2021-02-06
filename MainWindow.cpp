#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _workDirectory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);

    _scriptEngine = QSharedPointer<QJSEngine>::create();
    _scriptEngine->installExtensions(QJSEngine::ConsoleExtension);

    _settings = QSharedPointer<QSettings>::create(_workDirectory + "/settings.ini", QSettings::IniFormat);

    _testSequenceManager = new TestSequenceManager(_scriptEngine);

    QJSValue testSequenceManager = _scriptEngine->newQObject(_testSequenceManager);
    _scriptEngine->globalObject().setProperty("testSequenceManager", testSequenceManager);
    evaluateScriptsFromDirectory(_workDirectory + "/sequences");

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    //Choose device type
    QLabel* selectDeviceBoxLabel = new QLabel("Step 1. Choose test sequence", this);
    _selectDeviceModelBox = new QComboBox(this);
    _selectDeviceModelBox->setFixedSize(150, 30);
    _selectDeviceModelBox->addItems(_testSequenceManager->avaliableSequencesNames());
    _testSequenceManager->setCurrentSequence(_selectDeviceModelBox->currentText());
    connect(_selectDeviceModelBox, SIGNAL(currentTextChanged(const QString&)), _testSequenceManager, SLOT(setCurrentSequence(const QString&)));
    connect(_selectDeviceModelBox, &QComboBox::currentTextChanged, [=]()
    {
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
    });

    QHBoxLayout* selectDeviceLayout = new QHBoxLayout;
    selectDeviceLayout->addWidget(selectDeviceBoxLabel);
    selectDeviceLayout->addWidget(_selectDeviceModelBox);
    selectDeviceLayout->addStretch();

    mainLayout->addLayout(selectDeviceLayout);

    //Test functions list widget
    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setFixedWidth(200);
    _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
    mainLayout->addWidget(_testFunctionsListWidget);

    mainLayout->addStretch();

    //Start testing buttons
    QHBoxLayout* startTestingButtonsLayout = new QHBoxLayout;
    mainLayout->addLayout(startTestingButtonsLayout);

    startTestingButtonsLayout->addStretch();
    _startFullCycleTestingButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTestingButton->setFixedSize(180, 40);
    startTestingButtonsLayout->addWidget(_startFullCycleTestingButton);
    connect(_startFullCycleTestingButton, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Selected Test"), this);
    _startSelectedTestButton->setFixedSize(180, 40);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);
    startTestingButtonsLayout->addStretch();

    //Log widget
    _logWidget = new QListWidget(this);
    _logWidget->setFixedHeight(200);
    mainLayout->addWidget(_logWidget);

    //Database
    _db = new DataBase(_settings, this);
    _db->connectToDataBase();
    //_db->insertIntoTable("test", QDateTime::currentDateTime().toString());

    _jlink = new ConsoleProcess(_settings, this);
    QJSValue jlink = _scriptEngine->newQObject(_jlink);
    _scriptEngine->globalObject().setProperty("jlink", jlink);
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
   _jlink->startJLinkScript(_settings->value("Zhaga/Script", "olc_zhaga_software.jlink").toString());
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
    _jlink->startJLinkScript(_workDirectory + "/olc_zhaga_railtest.jlink");
    return;

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

QJSValue MainWindow::evaluateScriptFromFile(const QString &scriptFileName)
{
    QFile scriptFile(scriptFileName);
    scriptFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&scriptFile);
    in.setCodec("Utf-8");
    QJSValue scriptResult = _scriptEngine->evaluate(QString(in.readAll()));
    scriptFile.close();
    return scriptResult;
}

QList<QJSValue> MainWindow::evaluateScriptsFromDirectory(const QString& directoryName)
{
    QDir scriptsDir = QDir(directoryName, "*.js", QDir::Name, QDir::Files);
    QStringList fileNames = scriptsDir.entryList();
    QList<QJSValue> results;

    for (auto & i : fileNames)
    {
        results.push_back(evaluateScriptFromFile(scriptsDir.absoluteFilePath(i)));
    }

    return results;
}

QJSValue MainWindow::runScript(const QString& scriptName, const QJSValueList& args)
{
    return _scriptEngine->globalObject().property(scriptName).call(args);
}
