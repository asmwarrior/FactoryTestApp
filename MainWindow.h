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

#include "JLinkManager.h"
#include "Database.h"
#include "TestSequence.h"
#include "Logger.h"
#include "RailtestClient.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void startFullCycleTesting();

private:

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    void setCurrentJLinkIndex(int index);
    int getCurrentJLinkIndex();

    QSharedPointer<QJSEngine> _scriptEngine;

    QList<QThread*> _threads;
    QList<JLinkManager*> _JLinkList;

    RailtestClient* _rail;

    QString _workDirectory;
    TestSequenceManager* _testSequenceManager;


    QSharedPointer<QSettings> _settings;
    DataBase *_db;
    QSqlTableModel  *model;

    QSharedPointer<Logger> _logger;

    //--- GUI Elements ------------------------------------------------
    QComboBox* _selectSequenceBox;
    QListWidget* _testFunctionsListWidget;
    QPushButton* _startFullCycleTestingButton;
    QPushButton* _startSelectedTestButton;
    QSharedPointer<QListWidget> _logWidget;

//    QThread* thr1;
//    QList<TestObject1*> _obj;
};

#endif // MAINWINDOW_H
