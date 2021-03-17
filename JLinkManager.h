#pragma once

#include <QProcess>
#include <QSettings>
#include "Logger.h"

#include "JLinkSDK/JLinkARMDLL.h"


class JLinkManager : public QObject
{
    Q_OBJECT

    public:

    enum State {unknown, waitingTestResponse, connectionTested};

    explicit JLinkManager(const QSharedPointer<QSettings> &settings, QObject *parent = Q_NULLPTR);
    ~JLinkManager() Q_DECL_OVERRIDE;

    void setLogger(const QSharedPointer<Logger> &logger) {_logger = logger;}

    void setSN(const QString& serialNumber);
    QString getSN() const;

public slots:

    void selectByUSB();
    void open();
    void setDevice(const QString& device);
    void select(int interface = JLINKARM_TIF_SWD);
    void setSpeed(int speed);
    void connect();
    int erase();
    void reset();
    void go();
    void downloadFile(const QString& fileName, int adress);
    void close();

    void on_establishConnection();
    void on_startScript(const QString& scriptFile);
    void readStandardOutput();

signals:

    void establishConnection();
    void startScript(const QString& scriptFile);

private:

    void clearErrorBuffer();

    void logOut(const char* log) {_logger->logInfo(log);}
    void errorOut(const char* log) {_logger->logError(QString("JLINK ERROR: %1").arg(log));}

    QSharedPointer<QSettings> _settings;
    QSharedPointer<Logger> _logger;
    State _state = unknown;

    QString _device;
    int _targetInterface = JLINKARM_TIF_SWD;
    int _speed = 5000;
    int _hostInterface = JLINKARM_HOSTIF_USB;
    QString _SN; // JLink serial number
    QByteArray _errorBuffer;

    QProcess _proc;
};
