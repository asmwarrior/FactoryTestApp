#include "TestClient.h"

#include <QCoreApplication>
#include <QtEndian>
#include <QSerialPortInfo>
#include <math.h>

TestClient::TestClient(const QSharedPointer<QSettings> &settings, int no, QObject *parent)
    : QObject(parent),
      _portManager(this),
      _no(no),
      _settings(settings)
{
//    connect(&_portManager, &PortManager::responseRecieved, this, &TestClient::responseRecieved);

    connect(this, &TestClient::slotFullyTested, [this](int slot){emit dutFullyTested(_duts[slot]);});

    _duts[1] = dutTemplate;
    _duts[2] = dutTemplate;
    _duts[3] = dutTemplate;
}

TestClient::~TestClient()
{

}

void TestClient::setLogger(const QSharedPointer<Logger> &logger)
{
    _logger = logger;
    _portManager.setLogger(logger);
}

void TestClient::setPort(const QString &portName)
{
    _portManager.setPort(portName);
}

void TestClient::open()
{
    _portManager.open();

    setTimeout(1000);

    int csa = readCSA(0);

    if(csa > 0)
    {
        _isConnected = true;
    }

    else
        qDebug() << "MeasBoard" << _no << "is not connected";

    setTimeout(10000);
}

QStringList TestClient::availiblePorts() const
{
    auto availablePorts = QSerialPortInfo::availablePorts();
    QStringList list;
    for (auto & portInfo : availablePorts)
    {
        list.push_back(portInfo.serialNumber());
    }
    return list;
}

void TestClient::open(QString id)
{
    auto availablePorts = QSerialPortInfo::availablePorts();
    for (auto & portInfo : availablePorts)
    {
        if(portInfo.serialNumber() == id)
        {
            _portManager.setPort(portInfo.portName());
            _portManager.open();

            setTimeout(1000);
            int csa = readCSA(0);
            if(csa != -1)
            {
                _isConnected = true;
                _logger->logDebug(QString("Connection to the Measuring Board %1 has been established on %2").arg(_no).arg(portInfo.portName()));
            }

            else
                _logger->logDebug(QString("Connection to the Measuring Board %1 has NOT been established").arg(_no));

            setTimeout(10000);
            break;
        }
    }
}

void TestClient::setDutsNumbers(QString numbers)
{
    auto numberList = numbers.simplified().split("|");
    int slot = 1;
    for(auto & i : numberList)
    {
        _duts[slot]["no"] = i.toInt();
        slot++;
    }
}

void TestClient::setDutChecked(int no, bool checked)
{
    for(auto & dut : _duts)
    {
        if(dut["no"].toInt() == no)
        {
            dut["checked"] = checked;
            break;
        }
    }
}

void TestClient::addDutError(int slot, QString error)
{
    setDutProperty(slot, "error", _duts[slot]["error"].toString() + ";" + error);
}

void TestClient::setAllDutsChecked()
{
    for(auto & dut : _duts)
    {
        if(dut["state"].toBool())
            dut["checked"] = true;
    }
}

void TestClient::reverseDutsChecked()
{
    for(auto & dut : _duts)
    {
        if(dut["state"].toBool())
            dut["checked"] = !dut["checked"].toBool();
    }
}


int TestClient::switchSWD(int slot)
{
    _currentSlot = slot;
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t dut;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_SWD);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 1;
    pkt.dut = slot;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}


int TestClient::powerOn(int slot)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_POWER);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.state = 1;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::powerOff(int slot)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_POWER);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.state = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::readDIN(int slot, int DIN)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                din;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DIN);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.din = DIN;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::setDOUT(int slot, int DOUT)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                dout,
                state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_WRITE_DOUT);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 3;
    pkt.dut = slot;
    pkt.dout = DOUT;
    pkt.state = 1;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::clearDOUT(int slot, int DOUT)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                dout,
                state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_WRITE_DOUT);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 3;
    pkt.dut = slot;
    pkt.dout = DOUT;
    pkt.state = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response.last().toInt();
    }

    return -1;
}

int TestClient::readCSA(int gain)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t gain;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_CSA);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 1;
    pkt.gain = gain;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::readAIN(int slot, int AIN, int gain)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                ain,
                gain;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ANALOG);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 3;
    pkt.dut = slot;
    pkt.ain = AIN;
    pkt.gain = gain;

    _currentSlot = slot;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::daliOn()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_DALI);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 1;
    pkt.state = 1;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::daliOff()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t state;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_DALI);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 1;
    pkt.state = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::readDaliADC()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DALI_ADC);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::readDinADC(int slot, int DIN)
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
        uint8_t
                dut,
                din;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DIN_ADC);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.din = DIN;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::read24V()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_24V);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::read3V()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_3V);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

int TestClient::readTemperature()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_TEMP);
    pkt.h.sequence = ++_sequenceCounter;
    pkt.h.dataLen = 0;

    auto response = _portManager.slipCommand(QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
    }

    return -1;
}

QStringList TestClient::railtestCommand(int channel, const QByteArray &cmd)
{
    return _portManager.railtestCommand(channel, cmd);
}

void TestClient::testRadio(int slot, QString RfModuleId, int channel, int power, int minRSSI, int maxRSSI, int count)
{
    RailtestClient rf;
    _rssiValues.clear();

    connect(&rf, &RailtestClient::replyReceived, this, &TestClient::onRfReplyReceived);

    auto availablePorts = QSerialPortInfo::availablePorts();
    QString portName;
    for (auto & portInfo : availablePorts)
    {
        if(portInfo.serialNumber() == RfModuleId)
        {
            portName = portInfo.portName();
            break;
        }
    }

    if (!rf.open(portName))
    {
        _logger->logError("Cannot open serial port for reference radio module.");
        _logger->logDebug("Cannot open serial port for reference radio module.");
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + "Cannot open serial port for reference radio module.";
        return;
    }

    rf.syncCommand("reset", "", 2000);
    if (!rf.waitCommandPrompt())
    {
        _logger->logError("Timeout waiting reference radio module command prompt.");
        _logger->logDebug("Timeout waiting reference radio module command prompt.");
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + "Timeout waiting reference radio module command prompt.";
        return;
    }

    rf.syncCommand("rx", "0", 500);
    _rfRSSI = 255;
    _rfCount = 0;
    rf.syncCommand("setBleMode", "1", 500);
    rf.syncCommand("setBle1Mbps", "1", 500);
    rf.syncCommand("setChannel", QString().setNum(channel).toLocal8Bit(), 500);

    railtestCommand(slot, "rx 0");
    delay(500);

    railtestCommand(slot, "setBleMode 1");
    delay(500);

    railtestCommand(slot, "setBle1Mbps 1");
    delay(500);

    railtestCommand(slot, QString("setChannel %1").arg(channel).toLocal8Bit());
    delay(500);

    railtestCommand(slot, QString("setPower %1").arg(power).toLocal8Bit());
    delay(500);

    railtestCommand(slot, "setTxDelay 25");
    delay(500);

    rf.syncCommand("rx", "1", 500);

    railtestCommand(slot, QString("tx %1").arg(count).toLocal8Bit());
    delay(5000);

    double sumRSSI = 0;
    for (auto & i : _rssiValues)
    {
        sumRSSI += i;
    }
    double averageRSSI = sumRSSI / _rssiValues.size();

    double s0 = 0;
    foreach(int v, _rssiValues) s0 += pow(v - averageRSSI, 2);
    s0 = sqrt(s0 / (_rssiValues.size() - 1));
    _logger->logDebug(QString("For DUT %1 power: %2, packet recieved: %3, Average RSSI: %4, S0: %5.").arg(dutNo(slot)).arg(power).arg(_rssiValues.size()).arg(averageRSSI).arg(s0));

    if (_rfCount < (2 * count / 3))
    {
        _logger->logError(QString("Radio Interface testing failure for DUT %1.").arg(dutNo(slot)));
        _logger->logDebug(QString("Radio Interface failure for DUT %1: packet lost (%2).").arg(dutNo(slot)).arg(_rfCount));
        _duts[slot]["radioChecked"] = false;
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: packet lost (%1).").arg(_rfCount);
    }

    else if (averageRSSI < minRSSI)
    {
        _logger->logError(QString("Radio Interface testing failure for DUT %1.").arg(dutNo(slot)));
        _logger->logDebug(QString("Radio Interface failure for DUT %1: RSSI (%2) is out of bounds.").arg(dutNo(slot)).arg(averageRSSI));
        _duts[slot]["radioChecked"] = false;
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: RSSI (%1) is out of bounds.").arg(averageRSSI);
    }

    else
    {
        _logger->logSuccess(QString("Radio interface for DUT %1 has been tested successfully.").arg(dutNo(slot)));
        _duts[slot]["radioChecked"] = true;
    }
}

void TestClient::onRfReplyReceived(QString id, QVariantMap params)
{
    if (id == "rxPacket" && params.contains("rssi"))
    {
        bool ok;
        int rssi = params.value("rssi").toInt(&ok);

        if (ok)
        {
            ++_rfCount;
//            _logger->logDebug(QString("RSSI value recieved: %1").arg(rssi));
            _rssiValues.push_back(rssi);
            if (rssi < _rfRSSI)
                _rfRSSI = rssi;
        }
    }
}

void TestClient::delay(int msec)
{
    QTime expire = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
    }
}

void TestClient::resetDut(int slot)
{
    _duts[slot]["state"] = DutState::inactive;
    _duts[slot]["id"] = "";
    _duts[slot]["checked"] = false;
    _duts[slot]["voltageChecked"] = false;
    _duts[slot]["accelChecked"] = false;
    _duts[slot]["lightSensChecked"] = false;
    _duts[slot]["daliChecked"] = false;
    _duts[slot]["radioChecked"] = false;
    _duts[slot]["error"] = "";

    emit dutChanged(_duts[slot]);
}

void TestClient::setDutProperty(int slot, const QString &property, const QVariant &value)
{
    _duts[slot][property] = value;
    emit dutChanged(_duts[slot]);
}

QVariant TestClient::dutProperty(int slot, const QString &property)
{
    return _duts[slot][property];
}

bool TestClient::isActive() const
{
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            return true;
        }
    }

    return false;
}
