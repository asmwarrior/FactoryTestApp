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
#include "RailtestClient.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

public slots:

    void onSelectDeviceBoxCurrentTextChanged(const QString& text);
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

    QJSEngine _scriptEngine;
    ConsoleProcess* _jlink;

    QString _workDirectory;
    TestSequenceManager _testSequenceManager;

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);

    QSharedPointer<QSettings> _settings;
    DataBase *_db;
    QSqlTableModel  *model;

    //--- GUI Elements ------------------------------------------------
    QComboBox* _selectDeviceModelBox;
    QPushButton* _startFullCycleTesting;
    QListWidget *_logWidget;

    int _rfRSSI;
    int _rfCount;
};

#endif // MAINWINDOW_H
