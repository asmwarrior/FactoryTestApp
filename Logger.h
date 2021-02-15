#pragma once

#include <QListWidget>

class Logger : public QObject
{
    Q_OBJECT

public:

    explicit Logger(QObject *parent = nullptr);

    void setLogWidget(QListWidget* widget);

public slots:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

private:

    QListWidget* _logWidget;
};
