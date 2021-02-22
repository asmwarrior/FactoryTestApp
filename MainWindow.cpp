#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCompleter>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    thread()->setObjectName("Main Window thread");
    setStyleSheet("color: #424242; font-size:10pt;");    

    _settings = new QSettings(_workDirectory + "/settings.ini", QSettings::IniFormat, this);
    _settings->setValue("workDirectory", _workDirectory);

    _scriptEngine = _scriptEngine = new QJSEngine(this);
    _scriptEngine->installExtensions(QJSEngine::ConsoleExtension);

    _session = new SessionManager(this);
    _scriptEngine->globalObject().setProperty("session", _scriptEngine->newQObject(_session));

    _testSequenceManager = new TestSequenceManager(this);
    _logger = new Logger(this);
    _scriptEngine->globalObject().setProperty("testSequenceManager", _scriptEngine->newQObject(_testSequenceManager));

    evaluateScriptFromFile(_workDirectory + "/init.js");
    evaluateScriptsFromDirectory(_workDirectory + "/sequences");
    _scriptEngine->globalObject().setProperty("logger", _scriptEngine->newQObject(_logger));

    // Creating threads for run the tests for each test panel
    for (int i = 0; i < 5; i++)
    {
        auto newThread = new QThread(this);
        newThread->setObjectName(QString("Thread %1").arg(i + 1));
        _threads.push_back(newThread);
    }

    // Creating objects for controlling JLinks, Rail Test & Slip clients
    for (int i = 0; i < 5; i++)
    {
        auto newJlink = new JLinkManager(_settings);
        newJlink->setSN(_settings->value(QString("JLink/SN" + QString().setNum(i + 1))).toString());
        newJlink->setLogger(_logger);
        _JLinkList.push_back(newJlink);
        QJSValue jlink = _scriptEngine->newQObject(newJlink);
        _scriptEngine->globalObject().property("JlinkList").setProperty(i, jlink);

        auto railClient = new RailtestClient(_settings);
        railClient->setLogger(_logger);
        railClient->setPort(_settings->value(QString("Railtest/serial%1").arg(QString().setNum(i + 1))).toString());
        _railTestClientList.push_back(railClient);
        _railTestClientList[i]->moveToThread(_threads[i]);
        QJSValue rail = _scriptEngine->newQObject(railClient);
        _scriptEngine->globalObject().property("railTestClientList").setProperty(i, rail);

        auto slipClient = new SlipClient();
        slipClient->setLogger(_logger);
        slipClient->setPort(_settings->value(QString("Railtest/serial%1").arg(QString().setNum(i + 1))).toString());
        _slipClientList.push_back(slipClient);
        _slipClientList[i]->moveToThread(_threads[i]);
        QJSValue slip = _scriptEngine->newQObject(slipClient);
        _scriptEngine->globalObject().property("slipClientList").setProperty(i, slip);

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
    _headerLabel = new QLabel("HERE WE PLACE HEADER INFO", this);
    _headerLabel->setStyleSheet("font-size:10pt; font-weight: bold;");
    headerLayout->addWidget(_headerLabel);
    headerLayout->addStretch();

    //Next action hint
    _actionHintWidget = new ActionHintWidget(this);
    _actionHintWidget->showNormalHint(HINT_START);
    mainLayout->addWidget(_actionHintWidget);

    //Input session info and start session widgets
    leftPanelLayout->addSpacing(10);
    QLabel* sessionInfoLabel = new QLabel("<b>Step 1.</b> Enter session information", this);
    leftPanelLayout->addWidget(sessionInfoLabel);

    QFormLayout* sessionInfoLayout = new QFormLayout;
    _operatorNameEdit = new QLineEdit(this);
    _operatorNameEdit->setPlaceholderText("Enter Operator Name here");
    _operatorList = _settings->value("operatorList").toString().split("|");
    _operatorNameEdit->setCompleter(new QCompleter(_operatorList, this));
    _operatorNameEdit->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    _batchNumberEdit = new QLineEdit(this);
    _batchNumberEdit->setPlaceholderText("Enter Batch number here");
    _batchInfoEdit = new QLineEdit(this);
    _batchInfoEdit->setPlaceholderText("Enter Batch info here");
    sessionInfoLayout->addRow("Operator name", _operatorNameEdit);
    sessionInfoLayout->addRow("Batch number", _batchNumberEdit);
    sessionInfoLayout->addRow("Batch information", _batchInfoEdit);
    leftPanelLayout->addLayout(sessionInfoLayout);
    leftPanelLayout->addSpacing(20);

    QHBoxLayout* sessionButtonLayout = new QHBoxLayout;
    leftPanelLayout->addLayout(sessionButtonLayout);
    _newSessionButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/testOnly")), tr("Create new session"), this);
    _newSessionButton->setFixedSize(165, 40);
    _newSessionButton->setEnabled(false);
    sessionButtonLayout->addWidget(_newSessionButton);
    sessionButtonLayout->addStretch();

    _finishSessionButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/finish")), tr("Finish current session"), this);
    _finishSessionButton->setFixedSize(165, 40);
    _finishSessionButton->setEnabled(false);
    sessionButtonLayout->addWidget(_finishSessionButton);

    leftPanelLayout->addSpacing(30);

    //Choose method box
    QLabel* selectSequenceBoxLabel = new QLabel("<b>Step 2.</b> Choose test method", this);
    _selectMetodBox = new QComboBox(this);
    _selectMetodBox->setEnabled(false);
    _selectMetodBox->setFixedSize(350, 30);

    leftPanelLayout->addWidget(selectSequenceBoxLabel);
    leftPanelLayout->addWidget(_selectMetodBox);

    //Test functions list widget
    QLabel* testFunctionsListLabel = new QLabel("Avaliable testing steps:", this);
    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setEnabled(false);
    _testFunctionsListWidget->setFixedWidth(350);

    leftPanelLayout->addWidget(testFunctionsListLabel);
    leftPanelLayout->addWidget(_testFunctionsListWidget);
    leftPanelLayout->addStretch();

    //Start testing buttons
    QHBoxLayout* startTestingButtonsLayout = new QHBoxLayout;
    leftPanelLayout->addLayout(startTestingButtonsLayout);
    leftPanelLayout->addSpacing(9);

    _startFullCycleTestingButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/autoDownload")), tr("Start full cycle testing"), this);
    _startFullCycleTestingButton->setFixedSize(165, 40);
    _startFullCycleTestingButton->setEnabled(false);
    startTestingButtonsLayout->addWidget(_startFullCycleTestingButton);
    //connect(_startFullCycleTestingButton, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Selected Test"), this);
    _startSelectedTestButton->setFixedSize(165, 40);
    _startSelectedTestButton->setEnabled(false);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);

    //Test fixture representation widget
    _testFixtureWidget = new TestFixtureWidget(_session, this);
    _testFixtureWidget->setEnabled(false);
    middlePanelLayout->addWidget(_testFixtureWidget);
    middlePanelLayout->addStretch();

    //Info widgets
    _sessionInfoWidget = new SessionInfoWidget(_session, this);
    rightPanelLayout->addWidget(_sessionInfoWidget);

    _dutInfoWidget = new DutInfoWidget(_session, this);
    rightPanelLayout->addWidget(_dutInfoWidget);
    rightPanelLayout->addStretch();

    //Log widget
    _logWidget = new QListWidget(this);
    _logWidget->setFixedHeight(200);
    logLayout->addWidget(_logWidget);
    _logger->setLogWidget(_logWidget);

    QVBoxLayout* logWidgetControlsLayout = new QVBoxLayout;
    logLayout->addLayout(logWidgetControlsLayout);

    _clearLogWidgetButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/clear")), "", this);
    _clearLogWidgetButton->setFixedSize(32, 32);
    _clearLogWidgetButton->setToolTip("Clear log output");
    logWidgetControlsLayout->addWidget(_clearLogWidgetButton);

    _copyLogWidgetButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/copy")), "", this);
    _copyLogWidgetButton->setFixedSize(32, 32);
    _copyLogWidgetButton->setToolTip("Copy log output");
    logWidgetControlsLayout->addWidget(_copyLogWidgetButton);
    logWidgetControlsLayout->addStretch();

    //Widget for logging output of child processes
    _childProcessOutputLogWidget = new QListWidget(this);
    _childProcessOutputLogWidget->setFixedHeight(200);
    logLayout->addWidget(_childProcessOutputLogWidget);
    _logger->setChildProcessLogWidget(_childProcessOutputLogWidget);

    //Database
    _db = new DataBase(_settings, this);
    _db->connectToDataBase();
    //_db->insertIntoTable("test", QDateTime::currentDateTime().toString());

    //Connections
    connect(_operatorNameEdit, &QLineEdit::textEdited, [=](const QString& text)
    {
        if(!text.isEmpty() && !_batchNumberEdit->text().isEmpty() && !_session->isStarted())
        {
            _newSessionButton->setEnabled(true);
        }

        else
        {
            _newSessionButton->setEnabled(false);
        }
    });

    connect(_batchNumberEdit, &QLineEdit::textEdited, [=](const QString& text)
    {
        if(!text.isEmpty() && !_operatorNameEdit->text().isEmpty() && !_session->isStarted())
        {
            _newSessionButton->setEnabled(true);
        }

        else
        {
            _newSessionButton->setEnabled(false);
        }
    });

    connect(_selectMetodBox, SIGNAL(currentTextChanged(const QString&)), _testSequenceManager, SLOT(setCurrentSequence(const QString&)));
    connect(_selectMetodBox, &QComboBox::currentTextChanged, [=]()
    {
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
        if(_testFunctionsListWidget->count() > 0)
        {
            _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
        }
    });

    connect(_startSelectedTestButton, &QPushButton::clicked, [=]()
    {
        if(_testFunctionsListWidget->currentItem())
        {
            _testSequenceManager->runTestFunction(_testFunctionsListWidget->currentItem()->text());
        }
    });

    connect(_testFixtureWidget, &TestFixtureWidget::dutStateChanged, [=]()
    {
        _dutInfoWidget->showDutInfo(_session->getCurrentDut());
    });

    connect(_clearLogWidgetButton, &QPushButton::clicked, _logWidget, &QListWidget::clear);
    connect(_clearLogWidgetButton, &QPushButton::clicked, _childProcessOutputLogWidget, &QListWidget::clear);

    connect(_newSessionButton, &QPushButton::clicked, this, &MainWindow::startNewSession);
    connect(_finishSessionButton, &QPushButton::clicked, this, &MainWindow::finishSession);

//    int dutNo = 1;
//    for(auto & slip : _slipClientList)
//    {
//        for(int i = 1; i < 4; i++)
//        {
//            logger->logInfo(QString("Reading AIN for DUT %1").arg(dutNo));
//            dutNo++;
//            slip->readAIN(i, 3, 0);
//        }
//    }
}

MainWindow::~MainWindow()
{
    for(auto & jlink : _JLinkList)
    {
        delete jlink;
    }

    for(auto & rail : _railTestClientList)
    {
        rail->deleteLater();
    }

    for(auto & slip : _slipClientList)
    {
        slip->deleteLater();
    }

    for(auto & thread : _threads)
    {
        thread->quit();
        thread->deleteLater();
    }

    _settings->setValue("operatorList", _operatorList.join("|"));
}

void MainWindow::startNewSession()
{
    _session->setStarted(true);
    _session->setOperatorName(_operatorNameEdit->text().simplified());
    _session->setStartTime(QDateTime::currentDateTime().toString());
    _session->setBatchNumber(_batchNumberEdit->text());
    _session->setBatchInfo(_batchInfoEdit->text());

    _session->resetDutList();
    resetDutListInScriptEnv();

    //------------------------------------------------------------------------------------------
    _selectMetodBox->setEnabled(true);
    _selectMetodBox->clear();
    _selectMetodBox->addItems(_testSequenceManager->avaliableSequencesNames());

    _testSequenceManager->setCurrentSequence(_selectMetodBox->currentText());

    _testFunctionsListWidget->setEnabled(true);
    _testFunctionsListWidget->addItems(_testSequenceManager->currentSequenceFunctionNames());
    if(_testFunctionsListWidget->count() > 0)
    {
        _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
    }

    _startFullCycleTestingButton->setEnabled(true);
    _startSelectedTestButton->setEnabled(true);
    _newSessionButton->setEnabled(false);
    _testFixtureWidget->setEnabled(true);
    _finishSessionButton->setEnabled(true);

    _actionHintWidget->showNormalHint(HINT_CHOOSE_METHOD);
    _sessionInfoWidget->update();

    if(!_operatorList.contains(_session->getOperatorName(), Qt::CaseInsensitive))
    {
        _operatorList.push_back(_session->getOperatorName());
    }

    //--------------------------------------------------------------------------------------
    for(auto & jlink : _JLinkList)
    {
        jlink->testConnection();
    }
}

void MainWindow::finishSession()
{
    _session->setStarted(false);
    _session->setOperatorName("");
    _session->setStartTime("");
    _session->setBatchNumber("");
    _session->setBatchInfo("");

    _selectMetodBox->clear();
    _selectMetodBox->setEnabled(false);

    _testFunctionsListWidget->clear();
    _testFunctionsListWidget->setEnabled(false);

    _startFullCycleTestingButton->setEnabled(false);
    _startSelectedTestButton->setEnabled(false);
    _newSessionButton->setEnabled(false);
    _testFixtureWidget->setEnabled(false);
    _finishSessionButton->setEnabled(false);
    _operatorNameEdit->clear();
    _batchNumberEdit->clear();
    _batchInfoEdit->clear();
    _logWidget->clear();
    _childProcessOutputLogWidget->clear();

    _actionHintWidget->showNormalHint(HINT_START);
    _sessionInfoWidget->update();
}

void MainWindow::startFullCycleTesting()
{
    for (auto & funcName : _testSequenceManager->currentSequenceFunctionNames())
    {
        _testSequenceManager->runTestFunction(funcName);
    }
}

void MainWindow::resetDutListInScriptEnv()
{
    int counter = 0;
    for(auto & dut : _session->getDutList())
    {
        QJSValue currentDut = _scriptEngine->newQObject(dut);
        _scriptEngine->globalObject().property("dutList").setProperty(counter, currentDut);
        counter++;
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
