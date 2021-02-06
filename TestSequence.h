#pragma once

#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QSharedPointer>
#include <QJSEngine>
#include <QJSValue>

class TestSequenceManager : public QObject
{
    Q_OBJECT

public:

    struct TestFunction
    {
        QString functionName;
        QJSValue function;
    };


    TestSequenceManager(const QSharedPointer<QJSEngine>& scriptEngine);

    Q_INVOKABLE void addSequence(const QString& name);
    Q_INVOKABLE void addTestFunction(const QString& name, const QJSValue& function);
    QStringList avaliableSequencesNames() const;
    QStringList currentSequenceFunctionNames() const;
    void runTestFunction(const QString& name, const QJSValueList& args = {});

public slots:
    void setCurrentSequence(const QString& name);

private:

    QSharedPointer<QJSEngine> _scriptEngine;
    QString _currentSequence;
    QMap<QString, QList<TestFunction>> _sequences;
};
