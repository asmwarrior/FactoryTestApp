#include "Logger.h"

#include <QDebug>

Logger::Logger(QSettings *settings, SessionManager *session, QObject *parent)
    : QObject(parent),
      _settings(settings),
      _session(session),
      _logMutex(QMutex::Recursive),
      _debugLogMutex(QMutex::Recursive)
{
    //Database
    _db = new DataBase(_settings, this);
    _db->connectToDataBase();
    //_db->insertIntoTable("test", QDateTime::currentDateTime().toString());
}

void Logger::setLogWidget(QListWidget *widget)
{
    _logWidget = widget;
}

void Logger::setChildProcessLogWidget(QListWidget *widget)
{
    _debugLogWidget = widget;
}

void Logger::logInfo(const QString &message)
{
    QMutexLocker locker(&_logMutex);

    if(!_logWidget)
        return;

    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(QBrush(QColor("#9E9E9E")));
    item->setForeground(Qt::white);

    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void Logger::logError(const QString &message)
{
    QMutexLocker locker(&_logMutex);

    if(!_logWidget)
        return;

    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(QBrush(QColor("#e57373")));
    item->setForeground(Qt::white);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qCritical().noquote() << message;
}

void Logger::logSuccess(const QString &message)
{
    QMutexLocker locker(&_logMutex);

    if(!_logWidget)
        return;

    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(QBrush(QColor("#81C784")));
    item->setForeground(Qt::white);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void Logger::logDebug(const QString &message)
{
    QMutexLocker locker(&_debugLogMutex);

    if(!_debugLogWidget)
        return;

    _debugLogWidget->addItem(message);
    _debugLogWidget->scrollToBottom();
    qInfo().noquote() << message;
}
