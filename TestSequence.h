#pragma once

#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QPointer>

class TestSequence
{
public:

    TestSequence(const QFileInfo& sequenceFile);
    ~TestSequence();

    QVariant getDescriptionValue(const QString& valueName) const;

private:

    QPointer<QSettings> _description;


};

class TestSequenceManager
{
public:

    TestSequenceManager();
    void loadSequences(const QString& dirName);
    QStringList avaliableSequencesNames() const;

private:

    QList<TestSequence> _sequencesList;
};
