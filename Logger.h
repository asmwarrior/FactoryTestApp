#pragma once

#include <QListWidget>
#include <QMutexLocker>
#include <QSettings>

#include "SessionManager.h"

class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(const QSharedPointer<QSettings> &settings, SessionManager* session, QObject *parent = nullptr);

    void setLogWidget(QListWidget* widget);
    void setChildProcessLogWidget(QListWidget* widget);

public slots:

    void on_logInfo(const QString message);
    void on_logError(const QString message);
    void on_logSuccess(const QString message);

    void on_logDebug(const QString message);

signals:

    void logInfo(const QString message);
    void logError(const QString message);
    void logSuccess(const QString message);

    void logDebug(const QString message);

private:

    QSharedPointer<QSettings> _settings;
    SessionManager* _session;

    QMutex _logMutex;
    QMutex _debugLogMutex;
    QListWidget* _logWidget = nullptr;
    QListWidget* _debugLogWidget = nullptr;
};
