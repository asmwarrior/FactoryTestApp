#pragma once

#include <QListWidget>
#include <QMutexLocker>

class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(QObject *parent = nullptr);

    void setLogWidget(QListWidget* widget);
    void setChildProcessLogWidget(QListWidget* widget);

public slots:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

    void logChildProcessOutput(const QString &message);

private:

    QMutex _logMutex;
    QMutex _childProcessLogMutex;
    QListWidget* _logWidget;
    QListWidget* _childProcessLogWidget;
};
