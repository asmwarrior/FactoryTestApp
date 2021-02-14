#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //_workDirectory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation); // For release version
    //_workDirectory = "./"; //For test version
    _workDirectory = "../.."; //For development


    _settings = QSharedPointer<QSettings>::create(_workDirectory + "/settings.ini", QSettings::IniFormat);
    _settings->setValue("workDirectory", QDir(_workDirectory).absolutePath()); //Make name of work directory avaliable for other classes that use settings

    //_scriptEngine = QSharedPointer<QJSEngine>::create();
    _scriptEngine.installExtensions(QJSEngine::ConsoleExtension);

    _logger = QSharedPointer<Logger>::create();
    QJSValue logger = _scriptEngine.newQObject(_logger.get());
    _scriptEngine.globalObject().setProperty("logger", logger);

    _session = QSharedPointer<Session>::create();
//    QJSValue session = _scriptEngine.newQObject(_session.get());
//    _scriptEngine.globalObject().setProperty("session", session);

    test = QSharedPointer<TestClass>::create();
    QJSValue testjs = _scriptEngine.newQObject(test.get());
    _scriptEngine.globalObject().setProperty("session", testjs);

    _testSequenceManager = new TestSequenceManager();
    _testSequenceManager->setLogger(_logger);
    QJSValue testSequenceManager = _scriptEngine.newQObject(_testSequenceManager);
    _scriptEngine.globalObject().setProperty("testSequenceManager", testSequenceManager);
    evaluateScriptFromFile(_workDirectory + "/init.js");
    evaluateScriptsFromDirectory(_workDirectory + "/sequences");

    // Creating threads for run the tests for each test panel
    for (int i = 0; i < 5; i++)
    {
        auto newThread = new QThread(this);
        _threads.push_back(newThread);
    }

    // Creating objects for controlling JLinks and Rail Test clients
    for (int i = 0; i < 5; i++)
    {
        auto newJlink = new JLinkManager(_settings);
        newJlink->setSN(_settings->value(QString("JLink/SN" + QString().setNum(i + 1))).toString());
        newJlink->setLogger(_logger);
        _JLinksList.push_back(newJlink);
        //_JLinkList[i]->moveToThread(_threads[i]);
        QJSValue jlink = _scriptEngine.newQObject(newJlink);
        _scriptEngine.globalObject().property("JlinksList").setProperty(i, jlink);

        auto railClient = new RailtestClient(_settings, this);
        railClient->setLogger(_logger);
        railClient->open(_settings->value(QString("Railtest/serial%1").arg(QString().setNum(i + 1))).toString());
        _railTestClientsList.push_back(railClient);
        //_railTestClientsList[i]->moveToThread(_threads[i]);
        QJSValue rail = _scriptEngine.newQObject(railClient);
        _scriptEngine.globalObject().property("railTestClientsList").setProperty(i, rail);

        _threads[i]->start();
    }

//--- GUI Layouts---
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    mainLayout->addSpacing(30);
    QHBoxLayout* headerLayout = new QHBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(30);

    QHBoxLayout* panelsLayout = new QHBoxLayout;
    mainLayout->addLayout(panelsLayout);

    QVBoxLayout* leftPanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(leftPanelLayout);
    panelsLayout->addStretch();

    QVBoxLayout* middlePanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(middlePanelLayout);
    panelsLayout->addStretch();

    QVBoxLayout* rightPanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(rightPanelLayout);

    QHBoxLayout* logLayout = new QHBoxLayout;
    mainLayout->addLayout(logLayout);

    //Header info

    headerLayout->addStretch();
    _headerLabel = new QLabel("HERE WE PLACE HEADER INFO");
    _headerLabel->setStyleSheet("color: #595959; font-size:10pt; font-weight: bold;");
    headerLayout->addWidget(_headerLabel);
    headerLayout->addStretch();

    //Choose sequence box
    QLabel* selectSequenceBoxLabel = new QLabel("Step 1. Choose test sequence", this);
    _selectSequenceBox = new QComboBox(this);
    _selectSequenceBox->setFixedSize(350, 30);
    _selectSequenceBox->addItems(_testSequenceManager->avaliableSequencesNames());
    _testSequenceManager->setCurrentSequence(_selectSequenceBox->currentText());
    connect(_selectSequenceBox, SIGNAL(currentTextChanged(const QString&)), _testSequenceManager, SLOT(setCurrentSequence(const QString&)));
    connect(_selectSequenceBox, &QComboBox::currentTextChanged, [=]()
    {
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
        if(_testFunctionsListWidget->count() > 0)
        {
            _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
        }
    });

    leftPanelLayout->addWidget(selectSequenceBoxLabel);
    leftPanelLayout->addWidget(_selectSequenceBox);

    //Test functions list widget
    QLabel* testFunctionsListLabel = new QLabel("Avaliable testing steps:", this);
    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setFixedWidth(350);
    _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
    if(_testFunctionsListWidget->count() > 0)
    {
        _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
    }

    leftPanelLayout->addWidget(testFunctionsListLabel);
    leftPanelLayout->addWidget(_testFunctionsListWidget);
    leftPanelLayout->addStretch();

    //Start testing buttons
    QHBoxLayout* startTestingButtonsLayout = new QHBoxLayout;
    leftPanelLayout->addLayout(startTestingButtonsLayout);
    leftPanelLayout->addSpacing(9);

    _startFullCycleTestingButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTestingButton->setFixedSize(160, 40);
    startTestingButtonsLayout->addWidget(_startFullCycleTestingButton);
    //connect(_startFullCycleTestingButton, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Selected Test"), this);
    _startSelectedTestButton->setFixedSize(160, 40);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);
    connect(_startSelectedTestButton, &QPushButton::clicked, [=]()
    {
        if(_testFunctionsListWidget->currentItem())
        {
            _testSequenceManager->runTestFunction(_testFunctionsListWidget->currentItem()->text());
        }
    });

    //Test fixture representation widget
    _testFixtureWidget = new TestFixtureWidget(_session);
    middlePanelLayout->addWidget(_testFixtureWidget);
    middlePanelLayout->addStretch();

    //Info widgets
    _sessionInfoWidget = new SessionInfoWidget;
    rightPanelLayout->addWidget(_sessionInfoWidget);

    _dutInfoWidget = new DutInfoWidget(_session);
    rightPanelLayout->addWidget(_dutInfoWidget);
    connect(_testFixtureWidget, &TestFixtureWidget::dutStateChanged, [=]()
    {
        _dutInfoWidget->showDutInfo(_session->getCurrentDut());
    });
    rightPanelLayout->addStretch();

    //Log widget
    _logWidget = QSharedPointer<QListWidget>::create();
    _logWidget->setFixedHeight(200);
    logLayout->addWidget(_logWidget.get());
    _logger->setLogWidget(_logWidget);

    //Database
    _db = new DataBase(_settings, this);
    _db->connectToDataBase();
    //_db->insertIntoTable("test", QDateTime::currentDateTime().toString());
}

MainWindow::~MainWindow()
{
    for(auto & thread : _threads)
    {
        thread->quit();
    }

    for(auto & jlink : _JLinksList)
    {
        delete jlink;
    }

    for(auto & rail : _railTestClientsList)
    {
        delete rail;
    }

    delete _testSequenceManager;
}

void MainWindow::startFullCycleTesting()
{
    for (auto & funcName : _testSequenceManager->currentSequenceFunctionNames())
    {
        _testSequenceManager->runTestFunction(funcName);
    }
}

QJSValue MainWindow::evaluateScriptFromFile(const QString &scriptFileName)
{
    QFile scriptFile(scriptFileName);
    scriptFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&scriptFile);
    in.setCodec("Utf-8");
    QJSValue scriptResult = _scriptEngine.evaluate(QString(in.readAll()));
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
    return _scriptEngine.globalObject().property(scriptName).call(args);
}

void MainWindow::setCurrentJLinkIndex(int index)
{
    _scriptEngine.globalObject().setProperty("currentJLinkIndex", index);
}

int MainWindow::getCurrentJLinkIndex()
{
    return _scriptEngine.globalObject().property("currentJLinkIndex").toInt();
}
