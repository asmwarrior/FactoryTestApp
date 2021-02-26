#pragma once

#include <QListWidget>
#include <QMutexLocker>
#include <QSettings>

#include "SessionManager.h"
#include "Database.h"

class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(QSettings* settings, SessionManager* session, QObject *parent = nullptr);

    void setLogWidget(QListWidget* widget);
    void setChildProcessLogWidget(QListWidget* widget);

public slots:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

    void logDebug(const QString &message);

private:

    QSettings* _settings;
    SessionManager* _session;
    DataBase *_db;

    QMutex _logMutex;
    QMutex _childProcessLogMutex;
    QListWidget* _logWidget = nullptr;
    QListWidget* _childProcessLogWidget;
};
