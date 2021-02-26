#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QSerialPort>

#include "SlipProtocol.h"
#include "SessionManager.h"
#include "Logger.h"

class TestClient : public QObject
{
    Q_OBJECT

public:

    enum Mode {idleMode, railMode, slipMode};

    enum RailTestCommand {noCommand, readChipIdCommand};

    explicit TestClient(QSettings* settings, SessionManager* session, QObject *parent = nullptr);
    ~TestClient();

    void setLogger(Logger* logger);
    void setDutsNumbers(QList<int> numbers);

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    void open();
    void close();

    Q_INVOKABLE int getDutCount() const {return _dutsNumbers.size();}

private slots:

    void onSerialPortReadyRead();
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode);

    void sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW;
    void processResponsePacket();
    void onSlipPacketReceived(quint8 channel, QByteArray frame) Q_DECL_NOTHROW;

    void on_checkBoardCurrent();

    void on_sendRailtestCommand(int channel, const QByteArray& cmd, const QByteArray& args);
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

    void on_readChipId(int dut);
//    void on_testRadio();
//    void on_testAccelerometer();
//    void on_testLightSensor();
//    void on_testDALI();
//    void on_testGNSS();

signals:

    void checkBoardCurrent();

    //Slip commands
    void sendRailtestCommand(int channel, const QByteArray& cmd, const QByteArray& args);
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

    //Railtest commands
    void waitCommandPrompt(int timeout = 1000);
    void syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void readChipId(int dut);
    void testRadio();
    void testAccelerometer();
    void testLightSensor();
    void testDALI();
    void testGNSS();

private:

    void decodeFrame() Q_DECL_NOTHROW;
    void decodeRailtestReply(const QByteArray &reply);
    void processFrameFromRail(QByteArray frame);
    void delay(int msec);

    Mode _mode = idleMode;
    RailTestCommand _currentCommand = noCommand;
    QSettings* _settings;
    SessionManager* _session;
    Logger* _logger;

    QList<int> _dutsNumbers;

    QSerialPort _serial;
//    RailtestClient _rail;
//    SlipClient _slip;

    bool _frameStarted;
    QByteArray _recvBuffer;

    QByteArray _syncCommand;
    QVariantList _syncReplies;

    int _CSA = 0; //Board current (mA)
};

#endif // TESTCLIENT_H
