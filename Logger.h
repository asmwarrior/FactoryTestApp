#pragma once

#include <QListWidget>
#include <QSharedPointer>


class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(QObject *parent = nullptr);

    void setLogWidget(const QSharedPointer<QListWidget>& widget);

public slots:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

private:

    QSharedPointer<QListWidget> _logWidget;

};
