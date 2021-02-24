#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include "RailtestClient.h"
#include "SlipClient.h"

class TestClient : public QObject
{
    Q_OBJECT

public:

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

    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode);

signals:

    void checkBoardCurrent();

    //Slip commands
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

    //Railtest commands
    void waitCommandPrompt(int timeout = 1000);
    void syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void readChipId();
    void testRadio();
    void testAccelerometer();
    void testLightSensor();
    void testDALI();
    void testGNSS();

private:

    QSettings* _settings;
    SessionManager* _session;
    Logger* _logger;

    QList<int> _dutsNumbers;

    QSerialPort _serial;
    RailtestClient _rail;
    SlipClient _slip;
};

#endif // TESTCLIENT_H
