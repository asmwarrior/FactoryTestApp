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


    TestMethodManager(QObject *parent = nullptr);

    void setLogger(const QSharedPointer<Logger>& logger);

    Q_INVOKABLE void addMethod(const QString& name);
    Q_INVOKABLE void addFunctionToGeneralList(const QString& name, const QJSValue& function);
    Q_INVOKABLE void addFunctionToTestSequence(const QString& name, const QJSValue& function);
    QStringList avaliableSequencesNames() const;
    QStringList currentMethodGeneralFunctionNames() const;
    QStringList currentMethodSequenceFunctionNames() const;

public slots:
    void setCurrentMethod(const QString& name);
    void runTestFunction(const QString& name, const QJSValueList& args = {});

private:

    QSharedPointer<Logger> _logger;
    QString _currentMethod;
    QMap<QString, TestMethod> _methods;
};
