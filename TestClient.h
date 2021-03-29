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

    explicit TestClient(const QSharedPointer<QSettings> &settings, int no, QObject *parent = nullptr);
    ~TestClient();

    void setLogger(const QSharedPointer<Logger> &logger);
    void setDutsNumbers(QString numbers);

    void setPort(const QString& portName);
    void open();

    QMap<int, Dut> getDuts() {return _duts;}

public slots:

    QStringList availiblePorts() const;
    void open(QString id);

    int no() const {return _no;}

    bool isActive() const; //True, if at least one DUT connected
    bool isConnected() const {return _isConnected;}

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

    //SLIP commands

    int switchSWD(int slot);
    int powerOn(int slot);
    int powerOff(int slot);
    int readDIN(int slot, int DIN);
    int setDOUT(int slot, int DOUT);
    int clearDOUT(int slot, int DOUT);
    int readCSA(int gain);
    int readAIN(int slot, int AIN, int gain);
    int daliOn();
    int daliOff();
    int readDaliADC();
    int readDinADC(int slot, int DIN);
    int read24V();
    int read3V();
    int readTemperature();

    QStringList railtestCommand(int channel, const QByteArray &cmd);
    void testRadio(int slot, QString RfModuleId, int channel, int power, int minRSSI, int maxRSSI, int count);

    void setTimeout(int value) {_portManager.setTimeout(value);}

signals:

    void responseRecieved(QStringList response);

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

    QMap<int, Dut> _duts;

    bool _isConnected = false;
    int _currentSlot = 0;

    int _rfRSSI;
    int _rfCount;
};

#endif // TESTCLIENT_H
