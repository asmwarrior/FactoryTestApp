#include "TestSequence.h"

#include <QDebug>

TestSequenceManager::TestSequenceManager(const QSharedPointer<QJSEngine> &scriptEngine, QObject *parent) : QObject(parent), _scriptEngine(scriptEngine)
{

}

void TestSequenceManager::setLogger(const QSharedPointer<Logger> &logger)
{
    _logger = logger;
}

void TestSequenceManager::addSequence(const QString& name)
{
    _sequences.insert(name, {});
    setCurrentSequence(name);
}

void TestSequenceManager::setCurrentSequence(const QString &name)
{
    _currentSequence = name;
}

QStringList TestSequenceManager::avaliableSequencesNames() const
{
    return _sequences.keys();
}

QStringList TestSequenceManager::currentSequenceFunctionNames() const
{
    QStringList names;
    for(auto & i : _sequences[_currentSequence])
    {
        names.push_back(i.functionName);
    }

    return names;
}

void TestSequenceManager::runTestFunction(const QString &name, const QJSValueList &args)
{
    for(auto & i : _sequences[_currentSequence])
    {
        if(i.functionName == name)
        {
            i.function.call(args);
            break;
        }
    }
}

void TestSequenceManager::addTestFunction(const QString &name, const QJSValue &function)
{
    _sequences[_currentSequence].push_back({name, function});
}
