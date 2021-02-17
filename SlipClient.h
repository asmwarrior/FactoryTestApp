#ifndef __CAP_MB_SLIP_H__
#define __CAP_MB_SLIP_H__

#include <QThread>
#include <QSerialPort>
#include <QMetaMethod>
#include <QMutex>

#include "SlipProtocol.h"

class SlipClient : public QObject
{
    Q_OBJECT

public:
    explicit SlipClient(QObject *parent = Q_NULLPTR);
    ~SlipClient() Q_DECL_OVERRIDE;

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    void open();
    void close() Q_DECL_NOTHROW;

    inline bool isActive() const { return m_thread.isRunning(); }

    inline void lock() { m_cycleLock.lock(); }
    inline void unlock() { m_cycleLock.unlock(); }

    quint8 nextFrameId() Q_DECL_NOTHROW;

    void sendPacket(quint8 channel, const QByteArray &frame) Q_DECL_NOTHROW;

private:
    QThread m_thread;
    QThread *m_origin;
    QSerialPort m_serialPort;
    bool m_frameStarted;
    QByteArray m_recvBuffer;
    QMetaMethod m_sendMethod;

    QMutex m_attrLock;
    quint8 m_frameCnt;

    QMutex m_cycleLock;

    void cleanup() Q_DECL_NOTHROW;
    void decodeFrame() Q_DECL_NOTHROW;

private slots:
    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    void onSerialPortReadyRead() Q_DECL_NOTHROW;
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW;
    void onThreadFinished() Q_DECL_NOTHROW;

signals:
    void opened();
    void aboutToClose();
    void packetReceived(quint8 channel, QByteArray frame);
};

#endif // __CAP_MB_SLIP_H__
