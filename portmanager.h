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

    QStringList slipCommand(const QByteArray &frame, int msecs = 5000);
    QStringList railtestCommand(int channel, const QByteArray &cmd, int msecs = 5000);

public slots:

    bool open();
    void close();

private:

    QSerialPort _serial;

    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    QByteArray waitForFrame(int msecs);
    static bool decodeFrame(const QByteArray &frame, int &channel, QByteArray &message);
    QString getSerialError();
};

#endif // PORTMANAGER_H
