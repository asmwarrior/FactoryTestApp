#include "TestMethodManager.h"

#include <QDebug>

TestMethodManager::TestMethodManager(QObject *parent) : QObject(parent)
{
}

void TestMethodManager::setLogger(const QSharedPointer<Logger> &logger)
{
    _logger = logger;
}

void TestMethodManager::addMethod(const QString& name)
{
    _methods.insert(name, {});
    setCurrentMethod(name);
}

void TestMethodManager::setCurrentMethod(const QString &name)
{
    _currentMethod = name;
}

QStringList TestMethodManager::avaliableSequencesNames() const
{
    return _methods.keys();
}

QStringList TestMethodManager::currentMethodFunctionNames() const
{
    QStringList names;
    for(auto & i : _methods[_currentMethod].generalFunctionList)
    {
        names.push_back(i.functionName);
    }

    return names;
}

void TestMethodManager::runTestFunction(const QString &name, const QJSValueList &args)
{
    for(auto & i : _methods[_currentMethod].generalFunctionList)
    {
        if(i.functionName == name)
        {
            i.function.call(args);
            break;
        }
    }
}

void TestMethodManager::addFunctionToGeneralList(const QString &name, const QJSValue &function)
{
    _methods[_currentMethod].generalFunctionList.push_back({name, function});
}
