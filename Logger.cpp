#include "Logger.h"

#include <QDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::setLogWidget(const QSharedPointer<QListWidget> &widget)
{
    _logWidget = widget;
}

void Logger::logInfo(const QString &message)
{
    if(_logWidget.isNull())
        return;

    _logWidget->addItem(message);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void Logger::logError(const QString &message)
{
    if(_logWidget.isNull())
        return;

    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::darkRed);
    item->setForeground(Qt::white);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qCritical().noquote() << message;
}

void Logger::logSuccess(const QString &message)
{
    if(_logWidget.isNull())
        return;

    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::green);
    item->setForeground(Qt::black);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}
