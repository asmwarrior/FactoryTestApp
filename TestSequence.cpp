#include "TestSequence.h"

#include <QDebug>

TestSequenceManager::TestSequenceManager()
{

}

void TestSequenceManager::addSequence(const QString& name, const QStringList& functionsNames)
{
    _sequences.insert(name, functionsNames);
}

QStringList TestSequenceManager::avaliableSequencesNames() const
{
    return _sequences.keys();
}
