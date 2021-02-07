#pragma once

#include <QListWidget>
#include <QSharedPointer>


class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(QObject *parent = nullptr);

    void setLogWidget(const QSharedPointer<QListWidget>& widget);

    Q_INVOKABLE void logInfo(const QString &message);
    Q_INVOKABLE void logError(const QString &message);
    Q_INVOKABLE void logSuccess(const QString &message);

private:

    QSharedPointer<QListWidget> _logWidget;

};
