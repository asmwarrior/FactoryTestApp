#pragma once

#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QSharedPointer>
#include <QJSEngine>
#include <QJSValue>

#include "Logger.h"

class TestMethodManager : public QObject
{
    Q_OBJECT

public:

    struct TestFunction
    {
        QString functionName;
        QJSValue function;
    };


    struct TestMethod
    {
        QList<TestFunction> generalFunctionList; //All avaliable functions for this method, described in js file
        QList<TestFunction> testSequenceFunctionList; //List of functions called during the auto testing procedure
    };


    TestMethodManager(QSettings* settings, QObject *parent = nullptr);

    void setLogger(Logger* logger) {_logger = logger; _scriptEngine.globalObject().setProperty("logger", _scriptEngine.newQObject(_logger));}
    QJSEngine* getScriptEngine() {return &_scriptEngine;}

    Q_INVOKABLE void addMethod(const QString& name);
    Q_INVOKABLE void addFunctionToGeneralList(const QString& name, const QJSValue& function);
    Q_INVOKABLE void addFunctionToTestSequence(const QString& name, const QJSValue& function);
    QStringList avaliableSequencesNames() const;
    QStringList currentMethodGeneralFunctionNames() const;
    QStringList currentMethodSequenceFunctionNames() const;

public slots:
    void setCurrentMethod(const QString& name);
    void runTestFunction(const QString& name);

private:

    QJSValue evaluateScriptFromFile(const QString& scriptFileName);
    QList<QJSValue> evaluateScriptsFromDirectory(const QString& directoryName);
    QJSValue runScript(const QString& scriptName, const QJSValueList& args);

    QSettings* _settings;
    QJSEngine _scriptEngine;
    Logger* _logger;
    QString _currentMethod;
    QMap<QString, TestMethod> _methods;
};
