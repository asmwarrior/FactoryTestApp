#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QJSEngine>
#include <QThread>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QSqlTableModel>
#include <QModelIndex>

#include "SessionManager.h"
#include "TestSequence.h"
#include "Logger.h"
#include "JLinkManager.h"
#include "Database.h"
#include "RailtestClient.h"
#include "SlipClient.h"
#include "TestFixtureWidget.h"
#include "SessionInfoWidget.h"
#include "DutInfoWidget.h"
#include "ActionHintWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void startNewSession();
    void finishSession();
    void startFullCycleTesting();
    void resetDutListInScriptEnv();

private:

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    //QString _workDirectory = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation); // For release version
    //QString _workDirectory = QDir(".").absolutePath(); //For test version
    QString _workDirectory = QDir("../..").absolutePath(); //For development
    QSettings* _settings;
    QJSEngine* _scriptEngine;
    SessionManager* _session;
    TestSequenceManager* _testSequenceManager;
    Logger* _logger;

    QList<QThread*> _threads;
    QList<JLinkManager*> _JLinkList;
    QList<RailtestClient*> _railTestClientList;
    QList<SlipClient*> _slipClientList;

    QStringList _operatorList;

    DataBase *_db;
    QSqlTableModel  *model;

    //--- GUI Elements ------------------------------------------------

    QLabel* _headerLabel;
    QLineEdit* _operatorNameEdit;
    QLineEdit* _batchNumberEdit;
    QLineEdit* _batchInfoEdit;
    QPushButton* _newSessionButton;
    QPushButton* _finishSessionButton;
    QComboBox* _selectMetodBox;
    QListWidget* _testFunctionsListWidget;
    QPushButton* _startFullCycleTestingButton;
    QPushButton* _startSelectedTestButton;

    QListWidget* _logWidget;
    QPushButton* _clearLogWidgetButton;
    QPushButton* _copyLogWidgetButton;
    QListWidget* _childProcessOutputLogWidget;

    TestFixtureWidget* _testFixtureWidget;
    SessionInfoWidget* _sessionInfoWidget;
    DutInfoWidget* _dutInfoWidget;

    ActionHintWidget* _actionHintWidget;
    const QString HINT_START = "Place DUTs into the test fixture, enter information for the Step 1 and start test session";
    const QString HINT_CHOOSE_METHOD = "Choose a test method (Step 2), select DUTs (Step 3) and run full cycle testing (or single step testing) for the selected DUTs";
};

#endif // MAINWINDOW_H
