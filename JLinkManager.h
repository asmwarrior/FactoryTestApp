#pragma once

#include <QProcess>
#include <QSettings>

#include "Logger.h"

class JLinkManager : public QObject
{
    Q_OBJECT

    public:

    enum State {unknown, waitingTestResponse, connectionTested};

    explicit JLinkManager(QSettings* settings, QObject *parent = Q_NULLPTR);
    ~JLinkManager() Q_DECL_OVERRIDE;

    void setLogger(Logger* logger) {_logger = logger;}

    void setSN(const QString& serialNumber);
    QString getSN() const;

public slots:

        void on_testConnection();
        void on_startJlinkCommands(const QStringList& commands);

signals:

    void testConnection();
    void startJlinkCommands(const QStringList& commands);

private:

    void clearErrorBuffer();

    QSettings* _settings;
    Logger* _logger;
    State _state = unknown;
    QString _SN; // JLink serial number
    QByteArray _errorBuffer;
};
