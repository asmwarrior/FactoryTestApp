#ifndef __CAP_MB_SLIP_H__
#define __CAP_MB_SLIP_H__

#include <QThread>
#include <QSerialPort>
#include <QMetaMethod>
#include <QMutex>
#include <QtEndian>

#include "SlipProtocol.h"
#include "AppComponent.h"

class SlipClient : public QObject, public AppComponent
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

public slots:

    void sendDubugString(int channel, const QByteArray& string);
    void reset();
    void switchSWD(int DUT);
    void powerOn(int DUT);
    void powerOff(int DUT);
    void readDIN(int DUT, int DIN);
    void setDOUT(int DUT, int DOUT);
    void clearDOUT(int DUT, int DOUT);
    void readCSA(int gain);
    void readAIN(int DUT, int AIN, int gain);
    void configDebugSerial(int DUT, int baudRate = QSerialPort::Baud115200, unsigned char bits = QSerialPort::Data8, unsigned char parity = QSerialPort::NoParity, unsigned char stopBits = QSerialPort::OneStop);
    void DaliOn();
    void DaliOff();
    void readDaliADC();
    void readDinADC(int DUT, int DIN);
    void read24V();
    void read3V();
    void readTemperature();

private slots:

    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    void onSerialPortReadyRead() Q_DECL_NOTHROW;
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW;
    void onThreadFinished() Q_DECL_NOTHROW;
    void onSlipPacketReceived(quint8 channel, QByteArray frame) Q_DECL_NOTHROW;

signals:

    void opened();
    void aboutToClose();
    void packetReceived(quint8 channel, QByteArray frame);
};

#endif // __CAP_MB_SLIP_H__
