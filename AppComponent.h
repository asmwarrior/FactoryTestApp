#pragma once

#include <QSettings>
#include <QJSEngine>

#include "Dut.h"
#include "Session.h"
#include "TestSequence.h"
#include "Logger.h"

class AppComponent
{
public:

    AppComponent();
    ~AppComponent();

protected:

    void resetDutList();

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    static Session* session;
    static QList<Dut*> dutList;
    static TestSequenceManager* testSequenceManager;
    static Logger* logger;
    static QString workDirectory;
    static QSettings* settings;
    static QJSEngine* scriptEngine;

};
