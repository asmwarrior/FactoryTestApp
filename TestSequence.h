#pragma once

#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QSharedPointer>
#include <QJSEngine>
#include <QJSValue>

#include "Logger.h"

class TestSequenceManager : public QObject
{
    Q_OBJECT

public:

    struct TestFunction
    {
        QString functionName;
        QJSValue function;
    };


    TestSequenceManager(QObject *parent = nullptr);

    void setLogger(const QSharedPointer<Logger>& logger);

    Q_INVOKABLE void addSequence(const QString& name);
    Q_INVOKABLE void addTestFunction(const QString& name, const QJSValue& function);
    QStringList avaliableSequencesNames() const;
    QStringList currentSequenceFunctionNames() const;

public slots:
    void setCurrentSequence(const QString& name);
    void runTestFunction(const QString& name, const QJSValueList& args = {});

private:

    QSharedPointer<Logger> _logger;
    QString _currentSequence;
    QMap<QString, QList<TestFunction>> _sequences;
};
