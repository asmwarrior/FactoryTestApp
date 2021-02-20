#include "Logger.h"

#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::setLogWidget(QListWidget *widget)
{
    _logWidget = widget;
}

void Logger::setChildProcessLogWidget(QListWidget *widget)
{
    _childProcessLogWidget = widget;
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

void Logger::logChildProcessOutput(const QString &message)
{
    QMutexLocker locker(&_childProcessLogMutex);

    if(!_childProcessLogWidget)
        return;

    _childProcessLogWidget->addItem(message);
    _childProcessLogWidget->scrollToBottom();
    qInfo().noquote() << message;
}
