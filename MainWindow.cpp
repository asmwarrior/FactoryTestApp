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
    setStyleSheet("color: #424242; font-size:10pt;");

    // Creating threads for run the tests for each test panel
    for (int i = 0; i < 5; i++)
    {
        auto newThread = new QThread(this);
        _threads.push_back(newThread);
    }

    // Creating objects for controlling JLinks and Rail Test clients
    for (int i = 0; i < 5; i++)
    {
        auto newJlink = new JLinkManager(settings);
        newJlink->setSN(settings->value(QString("JLink/SN" + QString().setNum(i + 1))).toString());
        newJlink->setLogger(logger);
        _JLinksList.push_back(newJlink);
        //_JLinkList[i]->moveToThread(_threads[i]);
        QJSValue jlink = scriptEngine->newQObject(newJlink);
        scriptEngine->globalObject().property("JlinkList").setProperty(i, jlink);

        auto railClient = new RailtestClient(settings, this);
        railClient->setLogger(logger);
        railClient->open(settings->value(QString("Railtest/serial%1").arg(QString().setNum(i + 1))).toString());
        _railTestClientsList.push_back(railClient);
        //_railTestClientsList[i]->moveToThread(_threads[i]);
        QJSValue rail = scriptEngine->newQObject(railClient);
        scriptEngine->globalObject().property("railTestClientList").setProperty(i, rail);

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
    _operatorNameEdit = new QLineEdit;
    _operatorNameEdit->setPlaceholderText("Enter Operator Name here");
    _operatorList = settings->value("operatorList").toString().split("|");
    _operatorNameEdit->setCompleter(new QCompleter(_operatorList, this));
    _operatorNameEdit->completer()->setCaseSensitivity(Qt::CaseInsensitive);

    _batchNumberEdit = new QLineEdit;
    _batchNumberEdit->setPlaceholderText("Enter Batch number here");
    _batchInfoEdit = new QLineEdit;
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
    _selectMetodBox->addItems(testSequenceManager->avaliableSequencesNames());
    testSequenceManager->setCurrentSequence(_selectMetodBox->currentText());

    leftPanelLayout->addWidget(selectSequenceBoxLabel);
    leftPanelLayout->addWidget(_selectMetodBox);

    //Test functions list widget
    QLabel* testFunctionsListLabel = new QLabel("Avaliable testing steps:", this);
    _testFunctionsListWidget = new QListWidget(this);
    _testFunctionsListWidget->setEnabled(false);
    _testFunctionsListWidget->setFixedWidth(350);
    _testFunctionsListWidget->addItems(testSequenceManager->currentSequenceFunctionNames());
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
    _startFullCycleTestingButton->setFixedSize(165, 40);
    _startFullCycleTestingButton->setEnabled(false);
    startTestingButtonsLayout->addWidget(_startFullCycleTestingButton);
    //connect(_startFullCycleTestingButton, SIGNAL(clicked()), this, SLOT(startFullCycleTesting()));

    _startSelectedTestButton = new QPushButton(QIcon(QString::fromUtf8(":/icons/checked")), tr("Start Selected Test"), this);
    _startSelectedTestButton->setFixedSize(165, 40);
    _startSelectedTestButton->setEnabled(false);
    startTestingButtonsLayout->addWidget(_startSelectedTestButton);

    //Test fixture representation widget
    _testFixtureWidget = new TestFixtureWidget();
    _testFixtureWidget->setEnabled(false);
    middlePanelLayout->addWidget(_testFixtureWidget);
    middlePanelLayout->addStretch();

    //Info widgets
    _sessionInfoWidget = new SessionInfoWidget;
    rightPanelLayout->addWidget(_sessionInfoWidget);

    _dutInfoWidget = new DutInfoWidget();
    rightPanelLayout->addWidget(_dutInfoWidget);
    rightPanelLayout->addStretch();

    //Log widget
    _logWidget = new QListWidget(this);
    _logWidget->setFixedHeight(200);
    logLayout->addWidget(_logWidget);
    logger->setLogWidget(_logWidget);

    //Database
    _db = new DataBase(this);
    _db->connectToDataBase();
    //_db->insertIntoTable("test", QDateTime::currentDateTime().toString());

    //Connections
    connect(_operatorNameEdit, &QLineEdit::textEdited, [=](const QString& text)
    {
        if(!text.isEmpty() && !_batchNumberEdit->text().isEmpty() && !session->isStarted())
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
        if(!text.isEmpty() && !_operatorNameEdit->text().isEmpty() && !session->isStarted())
        {
            _newSessionButton->setEnabled(true);
        }

        else
        {
            _newSessionButton->setEnabled(false);
        }
    });

    connect(_selectMetodBox, SIGNAL(currentTextChanged(const QString&)), testSequenceManager, SLOT(setCurrentSequence(const QString&)));
    connect(_selectMetodBox, &QComboBox::currentTextChanged, [=]()
    {
        _testFunctionsListWidget->clear();
        _testFunctionsListWidget->addItems(testSequenceManager->currentSequenceFunctionNames());
        if(_testFunctionsListWidget->count() > 0)
        {
            _testFunctionsListWidget->setCurrentItem(_testFunctionsListWidget->item(0));
        }
    });

    connect(_startSelectedTestButton, &QPushButton::clicked, [=]()
    {
        if(_testFunctionsListWidget->currentItem())
        {
            testSequenceManager->runTestFunction(_testFunctionsListWidget->currentItem()->text());
        }
    });

    connect(_testFixtureWidget, &TestFixtureWidget::dutStateChanged, [=]()
    {
        _dutInfoWidget->showDutInfo(session->getCurrentDut());
    });

    connect(_newSessionButton, &QPushButton::clicked, [=]()
    {
        session->setStarted(true);
        session->setOperatorName(_operatorNameEdit->text().simplified());
        session->setStartTime(QDateTime::currentDateTime().toString());
        session->setBatchNumber(_batchNumberEdit->text());
        session->setBatchInfo(_batchInfoEdit->text());

        _selectMetodBox->setEnabled(true);
        _testFunctionsListWidget->setEnabled(true);
        _startFullCycleTestingButton->setEnabled(true);
        _startSelectedTestButton->setEnabled(true);
        _newSessionButton->setEnabled(false);
        _testFixtureWidget->setEnabled(true);
        _finishSessionButton->setEnabled(true);

        _actionHintWidget->showNormalHint(HINT_CHOOSE_METHOD);
        _sessionInfoWidget->update();

        if(!_operatorList.contains(session->getOperatorName(), Qt::CaseInsensitive))
        {
            _operatorList.push_back(session->getOperatorName());
        }
    });

    connect(_finishSessionButton, &QPushButton::clicked, [=]()
    {
        session->setStarted(false);
        session->setOperatorName("");
        session->setStartTime("");
        session->setBatchNumber("");
        session->setBatchInfo("");

        _selectMetodBox->setEnabled(false);
        _testFunctionsListWidget->setEnabled(false);
        _startFullCycleTestingButton->setEnabled(false);
        _startSelectedTestButton->setEnabled(false);
        _newSessionButton->setEnabled(false);
        _testFixtureWidget->setEnabled(false);
        _finishSessionButton->setEnabled(false);
        _operatorNameEdit->clear();
        _batchNumberEdit->clear();
        _batchInfoEdit->clear();

        _actionHintWidget->showNormalHint(HINT_START);
        _sessionInfoWidget->update();
    });
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

    settings->setValue("operatorList", _operatorList.join("|"));
}

void MainWindow::startFullCycleTesting()
{
    for (auto & funcName : testSequenceManager->currentSequenceFunctionNames())
    {
        testSequenceManager->runTestFunction(funcName);
    }
}

void MainWindow::setCurrentJLinkIndex(int index)
{
    scriptEngine->globalObject().setProperty("currentJLinkIndex", index);
}

int MainWindow::getCurrentJLinkIndex()
{
    return scriptEngine->globalObject().property("currentJLinkIndex").toInt();
}
