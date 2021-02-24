#ifndef __CAP_MB_SLIP_H__
#define __CAP_MB_SLIP_H__

#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaMethod>
#include <QMutex>
#include <QtEndian>

#include "SessionManager.h"
#include "SlipProtocol.h"
#include "Logger.h"

class SlipClient : public QObject
{
    Q_OBJECT

public:

    explicit SlipClient(QSerialPort& serial, SessionManager* session, QObject *parent = Q_NULLPTR);

    ~SlipClient() Q_DECL_OVERRIDE;

    void setLogger(Logger* logger) {_logger = logger;}
    void setDutsNumbers(QList<int> numbers);

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    quint8 nextFrameId() Q_DECL_NOTHROW;

    void sendPacket(quint8 channel, const QByteArray &frame) Q_DECL_NOTHROW;

    int getBoardCurrent() const {return _CSA;}

public slots:

    void on_checkBoardCurrent();

    void on_sendDubugString(int channel, const QByteArray& string);
    void on_reset();
    void on_switchSWD(int DUT);
    void on_powerOn(int DUT);
    void on_powerOff(int DUT);
    void on_readDIN(int DUT, int DIN);
    void on_setDOUT(int DUT, int DOUT);
    void on_clearDOUT(int DUT, int DOUT);
    void on_readCSA(int gain);
    void on_readAIN(int DUT, int AIN, int gain);
    void on_configDebugSerial(int DUT, int baudRate = QSerialPort::Baud115200, unsigned char bits = QSerialPort::Data8, unsigned char parity = QSerialPort::NoParity, unsigned char stopBits = QSerialPort::OneStop);
    void on_DaliOn();
    void on_DaliOff();
    void on_readDaliADC();
    void on_readDinADC(int DUT, int DIN);
    void on_read24V();
    void on_read3V();
    void on_readTemperature();

    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    void onSlipPacketReceived(quint8 channel, QByteArray frame) Q_DECL_NOTHROW;

private:

    void processResponsePacket();
    void cleanup() Q_DECL_NOTHROW;
    void decodeFrame() Q_DECL_NOTHROW;

    SessionManager* _session;
    Logger* _logger;
    QList<int> _dutsNumbers;

    QSerialPort& m_serialPort;
    bool m_frameStarted;
    QByteArray m_recvBuffer;

    quint8 m_frameCnt;

    int _CSA = 0; //Board current (mA)
};

#endif // __CAP_MB_SLIP_H__
