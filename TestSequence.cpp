#include "TestSequence.h"

#include <QDebug>

TestSequence::TestSequence(const QFileInfo &sequenceFile)
{
    _description = new QSettings(sequenceFile.absoluteFilePath(), QSettings::IniFormat);
}

TestSequence::~TestSequence()
{
}

QVariant TestSequence::getDescriptionValue(const QString &valueName) const
{
    return _description->value(valueName);
}

TestSequenceManager::TestSequenceManager()
{

}

void TestSequenceManager::loadSequences(const QString &dirName)
{
    QDir dir(dirName);
    QFileInfoList sequenceFilesNames = dir.entryInfoList(QStringList({"*.tseq"}), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (auto & fileInfo : sequenceFilesNames)
    {
        _sequencesList.push_back(TestSequence(fileInfo));
    }
}

QStringList TestSequenceManager::avaliableSequencesNames() const
{
    QStringList names;

    for(auto & i : _sequencesList)
    {
        names.push_back(i.getDescriptionValue("name").toString());
    }

    return names;
}
