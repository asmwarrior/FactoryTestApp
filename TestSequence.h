#pragma once

#include <QDir>
#include <QSettings>
#include <QStringList>
#include <QPointer>

class TestSequenceManager : public QObject
{
    Q_OBJECT

public:

    TestSequenceManager();
    void loadSequences(const QString& dirName);
    Q_INVOKABLE void addSequence(const QString& name, const QStringList& functionsNames);
    QStringList avaliableSequencesNames() const;

private:

    QMap<QString, QStringList> _sequences;
};
