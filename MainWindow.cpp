#include "MainWindow.h"

#include <QCoreApplication>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCompleter>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    thread()->setObjectName("Main Window thread");
    setStyleSheet("color: #424242; font-size:10pt;");    

    _settings = QSharedPointer<QSettings>::create(_workDirectory + "/settings.ini", QSettings::IniFormat);
    _settings->setValue("workDirectory", _workDirectory);

    _session = new SessionManager(_settings, this);
    _logger = QSharedPointer<Logger>::create(_settings, _session);
    _printerManager = new PrinterManager(_settings, this);
    _printerManager->setLogger(_logger);
    _methodManager = new TestMethodManager(_settings);
    _methodManager->setLogger(_logger);

    auto availablePorts = QSerialPortInfo::availablePorts();

    //Setting number of active measuring boards (max - 5)
    const int MAX_MEASBOARD_COUNT = 5;

    // Creating objects for controlling JLinks & Test clients
    for (int i = 0; i < MAX_MEASBOARD_COUNT; i++)
    {
        if(_settings->value(QString("TestBoard/state" + QString().setNum(i + 1))).toBool())
        {
            // Creating threads for run the tests for each test panel
            auto newThread = new QThread(this);
            newThread->setObjectName(QString("Thread %1").arg(i + 1));
            _threads.push_back(newThread);

            auto newJlink = new JLinkManager(_settings);
            newJlink->setSN(_settings->value(QString("JLink/SN" + QString().setNum(i + 1))).toString());
            newJlink->setLogger(_logger);
            _JLinkList.push_back(newJlink);
            if(_settings->value("multithread").toBool())
                _JLinkList.last()->moveToThread(_threads.last());

            _methodManager->scriptEngine()->globalObject().property("jlinkList").setProperty(i, _methodManager->scriptEngine()->newQObject(_JLinkList.last()));

            auto testClient = new TestClient(_settings, i + 1);
            testClient->setLogger(_logger);
            _testClientList.push_back(testClient);
            _testClientList.last()->setDutsNumbers(_settings->value(QString("TestBoard/duts" + QString().setNum(i + 1))).toString());

//            for (auto & portInfo : availablePorts)
//            {
//                if(portInfo.serialNumber() == _settings->value(QString("Railtest/serialID%1").arg(QString().setNum(i + 1))).toString())
//                {
//                    _testClientList.last()->setPort(portInfo.portName());
//                    break;
//                }
//            }

            if(_settings->value("multithread").toBool())
                _testClientList.last()->moveToThread(_threads.last());

            _methodManager->scriptEngine()->globalObject().property("testClientList")
                                                          .setProperty(_methodManager->scriptEngine()->globalObject().property("testClientList")
                                                          .property("length").toInt(), _methodManager->scriptEngine()->newQObject(_testClientList.last()));
            _threads.last()->start();
            delay(200);
        }
    }

//--- GUI Layouts---
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

//    mainLayout->addSpacing(30);
    QHBoxLayout* headerLayout = new QHBoxLayout;
    mainLayout->addLayout(headerLayout);
//    mainLayout->addSpacing(30);

    QHBoxLayout* panelsLayout = new QHBoxLayout;
    mainLayout->addLayout(panelsLayout);

    QVBoxLayout* leftPanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(leftPanelLayout);

    panelsLayout->addSpacing(30);

    QVBoxLayout* rightPanelLayout = new QVBoxLayout;
    panelsLayout->addLayout(rightPanelLayout);

    QHBoxLayout* logLayout = new QHBoxLayout;
    mainLayout->addLayout(logLayout);

    //Header info
    headerLayout->addStretch();
    _headerLabel = new QLabel("HERE WE PLACE HEADER INFO", this);
    _headerLabel->setStyleSheet("font-size:10pt; font-weight: bold;");
    QPixmap headerLogo(":/icons/logo");
    _headerLabel->setPixmap(headerLogo);
    headerLayout->addWidget(_headerLabel);
    headerLayout->addStretch();

    //Next action hint
    _actionHintWidget = new ActionHintWidget(this);
    _actionHintWidget->showNormalHint(HINT_START);
    _methodManager->scriptEngine()->globalObject().setProperty("actionHintWidget", _methodManager->scriptEngine()->newQObject(_actionHintWidget));
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
    _selectMetodBox->setFixedHeight(30);

    leftPanelLayout->addWidget(selectSequenceBoxLabel);
    leftPanelLayout->addWidget(_selectMetodBox);

    //Test functions list widget
    QHBoxLayout* functionsHeaderLayout = new QHBoxLayout;
    leftPanelLayout->addLayout(functionsHeaderLayout);

    QLabel* testFunctionsListLabel = new QLabel("Commands:", this);
    functionsHeaderLayout->addWidget(testFunctionsListLabel);
    functionsHeaderLayout->addStretch();

    QLabel* checkBoxLabel = new QLabel("Manual mode", this);
    functionsHeaderLayout->addWidget(checkBoxLabel);

    _manualCommandsCheckBox = new QCheckBox(this);
    _manualCommandsCheckBox->setChecked(false);
    functionsHeaderLayout->addWidget(_manualCommandsCheckBox);

    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setEnabled(false);
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

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Command"), this);
    _startSelectedTestButton->setFixedSize(165, 40);
    _startSelectedTestButton->setEnabled(false);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);

    //Session info widget

    QHBoxLayout* sessionWidgetLayout = new QHBoxLayout;
    rightPanelLayout->addLayout(sessionWidgetLayout);

    _sessionInfoWidget = new SessionInfoWidget(_session, this);
    sessionWidgetLayout->addWidget(_sessionInfoWidget);

    //Test fixture representation widget

    QHBoxLayout* dutsLayout = new QHBoxLayout;
    rightPanelLayout->addLayout(dutsLayout);
    _testFixtureWidget = new TestFixtureWidget(_session, this);
    _testFixtureWidget->setEnabled(false);
    dutsLayout->addWidget(_testFixtureWidget);

    //DUTs info widget

    QVBoxLayout* dutInfoWidgetLayout = new QVBoxLayout;
    dutsLayout->addLayout(dutInfoWidgetLayout);
    _dutInfoWidget = new DutInfoWidget(_session, this);
    dutInfoWidgetLayout->addSpacing(14);
    dutInfoWidgetLayout->addWidget(_dutInfoWidget);

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

//Connections

    connect(_operatorNameEdit, &QLineEdit::textEdited, [=](const QString& text)
    {
        if(!text.isEmpty() && !_batchNumberEdit->text().isEmpty())
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
        if(!text.isEmpty() && !_operatorNameEdit->text().isEmpty())
        {
            _newSessionButton->setEnabled(true);
        }

        else
        {
            _newSessionButton->setEnabled(false);
        }
    });

    connect(_selectMetodBox, &QComboBox::currentTextChanged, [=](QString methodName)
    {
        _methodManager->setCurrentMethod(methodName);

        _session->setMethod(methodName);
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(_methodManager->currentMethodGeneralFunctionNames());
        if(_testFunctionsListWidget->count() > 0)
        {
            _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
        }
    });

    connect(_manualCommandsCheckBox, &QCheckBox::clicked, _startSelectedTestButton, &QPushButton::setEnabled);
    connect(_manualCommandsCheckBox, &QCheckBox::clicked, _testFunctionsListWidget, &QPushButton::setEnabled);

    connect(_startSelectedTestButton, &QPushButton::clicked, this, &MainWindow::startSelectedFunction);
    connect(_testFunctionsListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::startSelectedFunction);

    connect(_testFixtureWidget, &TestFixtureWidget::dutClicked, _dutInfoWidget, &DutInfoWidget::setDutChecked);
    connect(_testFixtureWidget, &TestFixtureWidget::dutClicked, [=](int no)
    {
        _dutInfoWidget->showDutInfo(no);
    });

    for (auto & testClient : _testClientList)
    {
        connect(_testFixtureWidget, &TestFixtureWidget::selectAllButtonClicked, testClient, &TestClient::setAllDutsChecked);
    }

    for (auto & testClient : _testClientList)
    {
        connect(_testFixtureWidget, &TestFixtureWidget::reverseSelectionButtonClicked, testClient, &TestClient::reverseDutsChecked);
    }


    connect(_clearLogWidgetButton, &QPushButton::clicked, _logWidget, &QListWidget::clear);
    connect(_clearLogWidgetButton, &QPushButton::clicked, _childProcessOutputLogWidget, &QListWidget::clear);

    for (auto & testClient : _testClientList)
    {
        connect(testClient, &TestClient::dutChanged, _testFixtureWidget, &TestFixtureWidget::refreshButtonState, Qt::QueuedConnection);
        connect(_testFixtureWidget, &TestFixtureWidget::dutClicked, testClient, &TestClient::setDutChecked, Qt::QueuedConnection);
        connect(testClient, &TestClient::dutChanged, _dutInfoWidget, &DutInfoWidget::updateDut, Qt::QueuedConnection);
        connect(testClient, &TestClient::dutFullyTested, _session, &SessionManager::logDutInfo, Qt::QueuedConnection);
    }

    connect(_newSessionButton, &QPushButton::clicked, this, &MainWindow::startNewSession);
    connect(_finishSessionButton, &QPushButton::clicked, this, &MainWindow::finishSession);
    connect(_startFullCycleTestingButton, &QPushButton::clicked, this, &MainWindow::startFullCycleTesting);

    connect(_session, &SessionManager::printLabel, _printerManager, &PrinterManager::addLabel);

//    for (auto & portInfo : availablePorts)
//    {
//        _logger->logDebug(QString(portInfo.serialNumber() + " " + portInfo.portName()));
//    }
}

MainWindow::~MainWindow()
{
    _session->writeDutRecordsToDatabase();
    for(auto & jlink : _JLinkList)
    {
        jlink->deleteLater();
    }

    for(auto & test : _testClientList)
    {
        test->deleteLater();
    }

    for(auto & thread : _threads)
    {
        thread->quit();
        thread->deleteLater();
    }

    _settings->setValue("operatorList", _operatorList.join("|"));

    if(_selectMetodBox->isEnabled() && !_selectMetodBox->currentText().isEmpty())
        _settings->setValue("lastMethod", _selectMetodBox->currentText());
}

void MainWindow::startNewSession()
{

    setControlsEnabled(false);

    //------------------------------------------------------------------------------------------

    _session->setOperatorName(_operatorNameEdit->text().simplified());
    _session->setStartTime(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"));
    _session->setBatchNumber(_batchNumberEdit->text());
    _session->setBatchInfo(_batchInfoEdit->text());

    //------------------------------------------------------------------------------------------
    _selectMetodBox->setEnabled(true);
    _selectMetodBox->clear();
    _selectMetodBox->addItems(_methodManager->avaliableMethodsNames());

    auto lastMethod = _settings->value("lastMethod").toString();
    if(_methodManager->avaliableMethodsNames().contains(lastMethod))
    {
        _selectMetodBox->setCurrentText(lastMethod);
    }
    _session->setMethod(_selectMetodBox->currentText());
    _methodManager->setCurrentMethod(_selectMetodBox->currentText());
    _settings->setValue("lastMethod", _selectMetodBox->currentText());

    _manualCommandsCheckBox->setEnabled(true);
    _manualCommandsCheckBox->setChecked(false);

    _testFunctionsListWidget->setEnabled(false);
    _testFunctionsListWidget->clear();
    _testFunctionsListWidget->addItems(_methodManager->currentMethodGeneralFunctionNames());
    if(_testFunctionsListWidget->count() > 0)
    {
        _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
    }

    _startFullCycleTestingButton->setEnabled(true);
    _startSelectedTestButton->setEnabled(false);
    _testFixtureWidget->setEnabled(true);
    _finishSessionButton->setEnabled(true);

    _actionHintWidget->showNormalHint(HINT_CHOOSE_METHOD);
    _sessionInfoWidget->refresh();
    _testFixtureWidget->refreshButtonsState();

    if(!_operatorList.contains(_session->operatorName(), Qt::CaseInsensitive))
    {
        _operatorList.push_back(_session->operatorName());
    }
}

void MainWindow::finishSession()
{
    setControlsEnabled(false);
    _session->writeDutRecordsToDatabase();
    _session->clear();

    _settings->setValue("lastMethod", _selectMetodBox->currentText());
    _selectMetodBox->clear();
    _testFixtureWidget->reset();
    _operatorNameEdit->clear();
    _batchNumberEdit->clear();
    _batchInfoEdit->clear();
    _logWidget->clear();
    _childProcessOutputLogWidget->clear();

    _actionHintWidget->showNormalHint(HINT_START);
    _sessionInfoWidget->refresh();
    _testFixtureWidget->refreshButtonsState();

    _operatorNameEdit->setEnabled(true);
    _batchNumberEdit->setEnabled(true);
    _batchInfoEdit->setEnabled(true);
}

void MainWindow::startFullCycleTesting()
{
    _session->writeDutRecordsToDatabase();
    _session->increaseCyclesCount();
    _actionHintWidget->showProgressHint(HINT_DETECT_DUTS);

    setControlsEnabled(false);
    startFunction("Full cycle testing");

    _actionHintWidget->showProgressHint(HINT_READY);
    _session->writeDutRecordsToDatabase();
    setControlsEnabled(true);
    _newSessionButton->setEnabled(false);
    _operatorNameEdit->setEnabled(false);
    _batchNumberEdit->setEnabled(false);
    _batchInfoEdit->setEnabled(false);

    if(!_manualCommandsCheckBox->isChecked())
    {
        _testFunctionsListWidget->setEnabled(false);
        _startSelectedTestButton->setEnabled(false);
    }
}

void MainWindow::startSelectedFunction()
{
    if(_testFunctionsListWidget->currentItem())
    {
        QString functionName = _testFunctionsListWidget->currentItem()->text();
        startFunction(functionName);
    }
}

void MainWindow::startFunction(const QString &functionName)
{
    _methodManager->runTestFunction(functionName);
}

void MainWindow::setControlsEnabled(bool state)
{
    _operatorNameEdit->setEnabled(state);
    _batchNumberEdit->setEnabled(state);
    _batchInfoEdit->setEnabled(state);

    _newSessionButton->setEnabled(state);
    _finishSessionButton->setEnabled(state);

    _selectMetodBox->setEnabled(state);
    _manualCommandsCheckBox->setEnabled(state);
    _testFunctionsListWidget->setEnabled(state);
    _startFullCycleTestingButton->setEnabled(state);
    _startSelectedTestButton->setEnabled(state);

    _testFixtureWidget->setEnabled(state);
}

void MainWindow::delay(int msec)
{
    QTime expire = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
    }
}

Dut MainWindow::getDut(int no)
{
    for (auto & testClient : _testClientList)
    {
        for(auto & dut : testClient->getDuts())
        {
            if(dut["no"].toInt() == no)
                return dut;
        }
    }

    return dutTemplate;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(_finishSessionButton->isEnabled())
    {
        if(QMessageBox::question(this, "Capelon Test Station", "Current test session in progress! Are you shure you want to quit?") == QMessageBox::Yes)
        {
            QWidget::closeEvent(event);
        }

        else
        {
            event->ignore();
        }

        return;
    }

    if(!_printerManager->isQueueEmpty())
    {
        if(QMessageBox::question(this, "Capelon Test Station", "There are labels in the printer queue! Are you shure you want to quit?") == QMessageBox::Yes)
        {
            QWidget::closeEvent(event);
        }

        else
        {
            event->ignore();
        }

        return;
    }

    QWidget::closeEvent(event);
}
