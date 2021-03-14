#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QSerialPort>

#include "SlipProtocol.h"
#include "Logger.h"

class PortManager : public QObject
{
    Q_OBJECT

public:

    enum Mode {idleMode, railMode, slipMode};

    explicit PortManager(QObject *parent = nullptr);

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    void setLogger(Logger* logger) {_logger = logger;}

public slots:

    void open();
    void close();

    QStringList slipCommand(int channel, const QByteArray &frame);
    QStringList railtestCommand(int channel, const QByteArray &cmd);

signals:

    void responseRecieved(QStringList response);

private slots:

    void onSerialPortReadyRead();
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode);
    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    void processResponsePacket();
    void decodeFrame() Q_DECL_NOTHROW;
    void onSlipPacketReceived(quint8 channel, QByteArray frame) Q_DECL_NOTHROW;
    void decodeRailtestReply(const QByteArray &reply);
    void waitCommandFinished();

private:

    Logger* _logger;
    QSerialPort _serial;
    Mode _mode = idleMode;

    bool _frameStarted;
    QByteArray _recvBuffer;

    QByteArray _syncCommand;
    QVariantList _syncReplies;

    QStringList _response;
};

#endif // PORTMANAGER_H
