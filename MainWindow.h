#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJSEngine>
#include <QSettings>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include <QSqlTableModel>
#include <QModelIndex>

#include "ConsoleProcess.h"
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

    void downloadRailtest();
    void initDali(RailtestClient *rail);
    void testRadio(RailtestClient *rail);
    void testAccelerometer(RailtestClient *rail);
    void testLightSensor(RailtestClient *rail);
    void testDALI(RailtestClient *rail);
    void testGNSS(RailtestClient *rail);
    void startFullCycleTesting();
    void onRfReplyReceived(QString id, QVariantMap params);

private:

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    QSharedPointer<QJSEngine> _scriptEngine;
    ConsoleProcess* _jlink;

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

    int _rfRSSI;
    int _rfCount;
};

#endif // MAINWINDOW_H
