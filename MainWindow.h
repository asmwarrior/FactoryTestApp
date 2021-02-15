#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QJSEngine>
#include <QSettings>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include <QSqlTableModel>
#include <QModelIndex>

#include "AppComponent.h"
#include "JLinkManager.h"
#include "Database.h"
#include "TestSequence.h"
#include "Logger.h"
#include "RailtestClient.h"
#include "TestFixtureWidget.h"
#include "SessionInfoWidget.h"
#include "DutInfoWidget.h"
#include "Session.h"

class MainWindow : public QWidget, public AppComponent
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void startFullCycleTesting();

private:

//    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
//    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
//    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    void setCurrentJLinkIndex(int index);
    int getCurrentJLinkIndex();

    //QSharedPointer<Session> _session;
//    Session* _session;

//    QJSEngine _scriptEngine;

    QList<QThread*> _threads;
    QList<JLinkManager*> _JLinksList;
    QList<RailtestClient*> _railTestClientsList;

//    QString _workDirectory;
//    TestSequenceManager* _testSequenceManager;


//    QSharedPointer<QSettings> _settings;
    DataBase *_db;
    QSqlTableModel  *model;

//    QSharedPointer<Logger> _logger;

    //--- GUI Elements ------------------------------------------------

    QLabel* _headerLabel;
    QComboBox* _selectSequenceBox;
    QListWidget* _testFunctionsListWidget;
    QPushButton* _startFullCycleTestingButton;
    QPushButton* _startSelectedTestButton;
    QListWidget* _logWidget;
    TestFixtureWidget* _testFixtureWidget;
    SessionInfoWidget* _sessionInfoWidget;
    DutInfoWidget* _dutInfoWidget;
};

#endif // MAINWINDOW_H
