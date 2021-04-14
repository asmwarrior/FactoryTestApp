#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QSerialPort>

#include "SlipProtocol.h"
#include "Logger.h"

class PortManager : public QObject
{
    Q_OBJECT

public:

    explicit PortManager(QObject *parent = nullptr);

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    void setLogger(const QSharedPointer<Logger>& logger) { Q_UNUSED(logger); }

    QStringList slipCommand(const QByteArray &frame);
    QStringList railtestCommand(int channel, const QByteArray &cmd);

public slots:

    bool open();
    void close();
    void setTimeout(int value) { Q_UNUSED(value); }

private:

    QSerialPort _serial;

    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    QByteArray waitForFrame(int msecs = 3000);
    static bool decodeFrame(const QByteArray &frame, int &channel, QByteArray &message);
    static QStringList decodeSlipResponse(const QByteArray &frame);
    QString getSerialError();
};

#endif // PORTMANAGER_H
