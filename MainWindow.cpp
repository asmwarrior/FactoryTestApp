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
    _workDirectory = "../..";

    _settings = QSharedPointer<QSettings>::create(_workDirectory + "/settings.ini", QSettings::IniFormat);
    _settings->setValue("workDirectory", QDir(_workDirectory).absolutePath()); //Make name of work directory avaliable for other classes that use settings

    _scriptEngine = QSharedPointer<QJSEngine>::create(this);
    _scriptEngine->installExtensions(QJSEngine::ConsoleExtension);

    _logger = QSharedPointer<Logger>::create(this);
    QJSValue logger = _scriptEngine->newQObject(_logger.get());
    _scriptEngine->globalObject().setProperty("logger", logger);

    _testSequenceManager = new TestSequenceManager(_scriptEngine, this);
    _testSequenceManager->setLogger(_logger);
    QJSValue testSequenceManager = _scriptEngine->newQObject(_testSequenceManager);
    _scriptEngine->globalObject().setProperty("testSequenceManager", testSequenceManager);
    evaluateScriptsFromDirectory(_workDirectory + "/sequences");

    _jlink = new ConsoleProcess(_settings, this);
    _jlink->setLogger(_logger);
    QJSValue jlink = _scriptEngine->newQObject(_jlink);
    _scriptEngine->globalObject().setProperty("jlink", jlink);

    _rail = new RailtestClient(_settings, this);
    _rail->setLogger(_logger);
    QJSValue rail = _scriptEngine->newQObject(_rail);
    _scriptEngine->globalObject().setProperty("rail", rail);

//--- GUI ---
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

    QVBoxLayout* rightPanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(rightPanelLayout);

    QHBoxLayout* logLayout = new QHBoxLayout;
    mainLayout->addLayout(logLayout);

    //Header info

    headerLayout->addStretch();
    headerLayout->addWidget(new QLabel("HERE WE PLACE HEADER INFO"));
    headerLayout->addStretch();

    //Choose sequence box
    QLabel* selectSequenceBoxLabel = new QLabel("Step 1. Choose test sequence", this);
    _selectSequenceBox = new QComboBox(this);
    _selectSequenceBox->setFixedSize(300, 30);
    _selectSequenceBox->addItems(_testSequenceManager->avaliableSequencesNames());
    _testSequenceManager->setCurrentSequence(_selectSequenceBox->currentText());
    connect(_selectSequenceBox, SIGNAL(currentTextChanged(const QString&)), _testSequenceManager, SLOT(setCurrentSequence(const QString&)));
    connect(_selectSequenceBox, &QComboBox::currentTextChanged, [=]()
    {
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
    });

    leftPanelLayout->addWidget(selectSequenceBoxLabel);
    leftPanelLayout->addWidget(_selectSequenceBox);

    //Test functions list widget
    QLabel* testFunctionsListLabel = new QLabel("Avaliable testing steps:", this);
    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setFixedWidth(300);
    _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());

    leftPanelLayout->addWidget(testFunctionsListLabel);
    leftPanelLayout->addWidget(_testFunctionsListWidget);
    leftPanelLayout->addStretch();

    //Start testing buttons
    QHBoxLayout* startTestingButtonsLayout = new QHBoxLayout;
    leftPanelLayout->addLayout(startTestingButtonsLayout);

    _startFullCycleTestingButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTestingButton->setFixedSize(148, 40);
    startTestingButtonsLayout->addWidget(_startFullCycleTestingButton);
    connect(_startFullCycleTestingButton, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Selected Test"), this);
    _startSelectedTestButton->setFixedSize(148, 40);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);
    connect(_startSelectedTestButton, &QPushButton::clicked, [=](){
        _testSequenceManager->runTestFunction(_testFunctionsListWidget->currentItem()->text());
    });

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
    delete _testSequenceManager;
    delete _jlink;
    delete _rail;
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
