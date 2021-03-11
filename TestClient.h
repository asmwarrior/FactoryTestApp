#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include "SlipProtocol.h"
#include "PortManager.h"
#include "JLinkManager.h"
#include "TestMethodManager.h"
#include "SessionManager.h"
#include "Logger.h"
#include "RailtestClient.h"

class TestClient : public QObject
{
    Q_OBJECT

public:


    enum DutState {inactive, untested, tested, warning};
    enum RailTestCommand {noCommand, readChipIdCommand, accelCommand, lightSensCommand, daliCommand};

    explicit TestClient(QSettings* settings, SessionManager* session, int no, QObject *parent = nullptr);
    ~TestClient();

    void setLogger(Logger* logger);
    void setDutsNumbers(QString numbers);

    void setPort(const QString& portName);
    void open();

    QMap<int, Dut> getDuts() {return _duts;}

public slots:
    void on_test() {qDebug() << thread();}

    int no() const {return _no;}

    void setActive(bool state) {_isActive = state;}
    bool isActive() const; //True, if at least one DUT connected

//    int currentCSA() {return _CSA;}
    int dutsCount() const {return _duts.size();}
    Dut dut(int slot) const {return _duts[slot];}
    void setCurrentSlot(int slot) {_currentSlot = slot;}

    int dutNo(int slot) const {return _duts[slot]["no"].toInt();}

    int dutState(int slot) const {return _duts[slot]["state"].toInt();}
    void setDutState(int slot, int state) {_duts[slot]["state"] = state;}

    bool isDutAvailable(int slot) const {return _duts[slot]["state"].toBool();}
    bool isDutChecked(int slot) const {return _duts[slot]["checked"].toBool();}
    void setDutChecked(int no, bool checked);
    void setAllDutsChecked();
    void reverseDutsChecked();
//    void checkTestingCompletion();

    //----

    void on_delay(int msec);

    void on_resetDut(int slot);
    void on_setDutProperty(int slot, const QString& property, const QVariant& value);

//    void on_checkDutsCurrent();
//    void on_startTesting();

//    void on_sendRailtestCommand(int channel, const QByteArray& cmd, const QByteArray& args);
//    void on_reset();
//    void on_switchSWD(int DUT);
//    void on_powerOn(int DUT);
//    void on_powerOff(int DUT);
//    void on_readDIN(int DUT, int DIN);
//    void on_setDOUT(int DUT, int DOUT);
//    void on_clearDOUT(int DUT, int DOUT);
    int readCSA(int gain);
//    void on_readAIN(int DUT, int AIN, int gain);
//    void on_configDebugSerial(int DUT, int baudRate = QSerialPort::Baud115200, unsigned char bits = QSerialPort::Data8, unsigned char parity = QSerialPort::NoParity, unsigned char stopBits = QSerialPort::OneStop);
//    void on_DaliOn();
//    void on_DaliOff();
//    void on_readDaliADC();
//    void on_readDinADC(int DUT, int DIN);
//    void on_read24V();
//    void on_read3V();
//    void on_readTemperature();

//    void on_readChipId(int slot);
//    void on_testRadio(int slot);
//    void on_testAccelerometer(int slot);
//    void on_testLightSensor(int slot);
//    void on_testDALI();
//    void on_testGNSS();

    void onRfReplyReceived(QString id, QVariantMap params);

signals:
    void test();

//    void checkDutsCurrent();
//    void startTesting();
    void call_delay(int msec);

    void resetDut(int slot);
    void setDutProperty(int slot, const QString& property, const QVariant& value);
    void runTestFunction(const QString &name);

    void dutChanged(Dut);
    void dutFullyTested(Dut);
    void commandSequenceStarted();
    void commandSequenceFinished();

    //Slip commands
    void call_sendRailtestCommand(int channel, const QByteArray& cmd, const QByteArray& args);
    void call_reset();
    void call_switchSWD(int DUT);
    void call_powerOn(int DUT);
    void call_powerOff(int DUT);
    void call_readDIN(int DUT, int DIN);
    void call_setDOUT(int DUT, int DOUT);
    void call_clearDOUT(int DUT, int DOUT);
    void call_readCSA(int gain);
    void call_readAIN(int DUT, int AIN, int gain);
    void call_daliOn();
    void call_daliOff();
    void call_readDaliADC();
    void call_readDinADC(int DUT, int DIN);
    void call_read24V();
    void call_read3V();
    void call_readTemperature();

    //Railtest commands
    void waitCommandPrompt(int timeout = 1000);
    void syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void readChipId(int dut);
    //void testRadio();
    void testAccelerometer(int slot);
    void testLightSensor(int slot);
    void testDALI();
    void testGNSS();

private:



//    JLinkManager* _jlinkManager;
//    TestMethodManager* _methodManager;

    PortManager _portManager;
    int _no;
    RailTestCommand _currentCommand = noCommand;
    QSettings* _settings;
    SessionManager* _session;
    Logger* _logger;

    QMap<int, Dut> _duts;

    bool _isActive = false; //True, if at least one DUT connected
    int _CSA = -1; //Board current (mA)
    int _currentSlot = 0;
    QString _currentChipID = "";
    int _currentVoltage = 0;
    bool _currentAccelChecked = false;
    bool _currentLightSensChecked = false;
    bool _currentDaliChecked = false;
    QString _currentError = "";

    int _rfRSSI;
    int _rfCount;
};

#endif // TESTCLIENT_H
