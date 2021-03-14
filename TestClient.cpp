#include "TestClient.h"

#include <QCoreApplication>
#include <QtEndian>

TestClient::TestClient(QSettings *settings, SessionManager *session, int no, QObject *parent)
    : QObject(parent),
      _portManager(this),
      _no(no),
      _settings(settings),
      _session(session)
{
    connect(this, &TestClient::test, this, &TestClient::on_test);
    connect(&_portManager, &PortManager::responseRecieved, this, &TestClient::responseRecieved);

    connect(this, &TestClient::slotFullyTested, [this](int slot){emit dutFullyTested(_duts[slot]);});

    _duts[1] = dutTemplate;
    _duts[2] = dutTemplate;
    _duts[3] = dutTemplate;
}

TestClient::~TestClient()
{

}

void TestClient::setLogger(Logger *logger)
{
    _logger = logger;
}

void TestClient::setPort(const QString &portName)
{
    _portManager.setPort(portName);
}

void TestClient::open()
{
    _portManager.open();
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
    pkt.h.sequence = 1;
    pkt.h.dataLen = 1;
    pkt.dut = slot;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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
    pkt.h.sequence = 2;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.state = 1;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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
    pkt.h.sequence = 3;
    pkt.h.dataLen = 2;
    pkt.dut = slot;
    pkt.state = 0;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

    if(response.size())
    {
        return response[0].toInt();
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
    pkt.h.sequence = 7;
    pkt.h.dataLen = 1;
    pkt.gain = gain;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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
    pkt.h.sequence = 8;
    pkt.h.dataLen = 3;
    pkt.dut = slot;
    pkt.ain = AIN;
    pkt.gain = gain;

    _currentSlot = slot;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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
    pkt.h.sequence = 10;
    pkt.h.dataLen = 1;
    pkt.state = 1;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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
    pkt.h.sequence = 11;
    pkt.h.dataLen = 1;
    pkt.state = 0;

    auto response = _portManager.slipCommand(0, QByteArray((char*)&pkt, sizeof(pkt)));

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

void TestClient::testRadio(int slot)
{
    RailtestClient rf;

    connect(&rf, &RailtestClient::replyReceived, this, &TestClient::onRfReplyReceived);

    if (!rf.open(_settings->value("Railtest/rf_serial").toString()))
    {
        _logger->logError("Cannot open serial port for reference radio module!");
        return;
    }

    rf.syncCommand("reset", "", 3000);
    if (!rf.waitCommandPrompt())
    {
        _logger->logError("Timeout waiting reference radio module command prompt!");
        return;
    }

    rf.syncCommand("rx", "0", 1000);
    _rfRSSI = 255;
    _rfCount = 0;
    rf.syncCommand("setBleMode", "1", 1000);
    rf.syncCommand("setBle1Mbps", "1", 1000);
    rf.syncCommand("setChannel", "19", 1000);

    railtestCommand(slot, "rx 0");
//    on_delay(1000);

    railtestCommand(slot, "setBleMode 1");
//    on_delay(1000);

    railtestCommand(slot, "setBle1Mbps 1");
//    on_delay(1000);

    railtestCommand(slot, "setChannel 19");
//    on_delay(1000);

    railtestCommand(slot, "setPower 80");
//    on_delay(1000);

    rf.syncCommand("rx", "1", 1000);

    railtestCommand(slot, "tx 11");
//    on_delay(5000);

    if (_rfCount < 8)
    {
        _logger->logError(QString("Radio Interface failure: packet lost (%1)!").arg(_rfCount));
        _duts[slot]["radioChecked"] = false;
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: packet lost (%1)!").arg(_rfCount);
    }

    else if (_rfRSSI < -50 || _rfRSSI > 20)
    {
        _logger->logError(QString("Radio Interface failure: RSSI (%1) is out of bounds!").arg(_rfRSSI));
        _duts[slot]["radioChecked"] = false;
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: RSSI (%1) is out of bounds!").arg(_rfRSSI);
    }

    else
    {
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
            if (rssi < _rfRSSI)
                _rfRSSI = rssi;
        }
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
