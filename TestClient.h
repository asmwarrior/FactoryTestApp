#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QSemaphore>

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

    explicit TestClient(const QSharedPointer<QSettings> &settings, const QSharedPointer<QSemaphore>& semaphore, int no, QObject *parent = nullptr);
    ~TestClient();

    void setLogger(const QSharedPointer<Logger> &logger);
    void setDutsNumbers(QString numbers);

    void setPort(const QString& portName);
    void open();

    QMap<int, Dut> getDuts() {return _duts;}

public slots:
    void on_test();

    int no() const {return _no;}

    void setActive(bool state) {_isActive = state;}
    bool isActive() const; //True, if at least one DUT connected

    int dutsCount() const {return _duts.size();}
    Dut dut(int slot) const {return _duts[slot];}
    void setCurrentSlot(int slot) {_currentSlot = slot;}

    void setDutProperty(int slot, const QString& property, const QVariant& value);
    QVariant dutProperty(int slot, const QString& property);

    int dutNo(int slot) const {return _duts[slot]["no"].toInt();}

    int dutState(int slot) const {return _duts[slot]["state"].toInt();}
    void setDutState(int slot, int state) {_duts[slot]["state"] = state;}

    bool isDutAvailable(int slot) const {return _duts[slot]["state"].toBool();}
    bool isDutChecked(int slot) const {return _duts[slot]["checked"].toBool();}
    void setDutChecked(int no, bool checked);

    void addDutError(int slot, QString error);

    void setAllDutsChecked();
    void reverseDutsChecked();

    void resetDut(int slot);

    //---------

    void acquireSemaphore() {_syncSemaphore->acquire();}
    void releaseSemaphore() {_syncSemaphore->release();}
    void waitForResponse();
    QStringList lastResponse() const {return _lastResponse;}
    int lastResponseInt() const
    {
        if(_lastResponse.size())
        {
            return _lastResponse[0].toInt();
        }

        return -1;
    }

    //---------


    //SLIP commands

    int switchSWD(int slot);
    int powerOn(int slot);
    int powerOff(int slot);
//    void on_readDIN(int DUT, int DIN);
//    void on_setDOUT(int DUT, int DOUT);
//    void on_clearDOUT(int DUT, int DOUT);
    int readCSA(int gain);
    int readAIN(int slot, int AIN, int gain);
//    void on_configDebugSerial(int DUT, int baudRate = QSerialPort::Baud115200, unsigned char bits = QSerialPort::Data8, unsigned char parity = QSerialPort::NoParity, unsigned char stopBits = QSerialPort::OneStop);
    int daliOn();
    int daliOff();
//    void on_readDaliADC();
//    void on_readDinADC(int DUT, int DIN);
//    void on_read24V();
//    void on_read3V();
//    void on_readTemperature();

    QStringList railtestCommand(int channel, const QByteArray &cmd);
    void testRadio(int slot);

signals:

    void test();
    void responseRecieved(QStringList response);

    //--- SLIP commands calls ------------------------

    void call_switchSWD(int slot);
    void call_powerOn(int slot);
    void call_powerOff(int slot);
    void call_readDIN(int slot, int DIN);
    void call_setDOUT(int slot, int DOUT);
    void call_clearDOUT(int slot, int DOUT);
    void call_readCSA(int gain);
    void call_readAIN(int slot, int AIN, int gain);
    void call_daliOn();
    void call_daliOff();
    void call_readDaliADC();
    void call_readDinADC(int slot, int DIN);
    void call_read24V();
    void call_read3V();
    void call_readTemperature();

    //--- Railtest command call ---------------------

    void call_railtestCommand(int channel, const QByteArray &cmd);

    //-----------------------------------------------

    void dutChanged(Dut);
    void dutFullyTested(Dut);
    void slotFullyTested(int);
    void commandSequenceStarted();
    void commandSequenceFinished();

private slots:

    void onRfReplyReceived(QString id, QVariantMap params);
    void delay(int msec);

private:

    PortManager _portManager;
    int _no;
    QSharedPointer<QSettings> _settings;
    QSharedPointer<Logger> _logger;
    QSharedPointer<QSemaphore> _syncSemaphore;

    QMap<int, Dut> _duts;

    bool _isActive = false; //True, if at least one DUT connected
    int _currentSlot = 0;
    QStringList _lastResponse;
    bool _isWaitingForResponse = false;

    int _rfRSSI;
    int _rfCount;
};

#endif // TESTCLIENT_H
