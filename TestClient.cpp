#include "TestClient.h"

#include <QCoreApplication>
#include <QtEndian>
#include <QThread>

static constexpr char
    END_SLIP_OCTET  = 0xC0,
    END_SUBS_OCTET  = 0xDC,
    ESC_SLIP_OCTET  = 0xDB,
    ESC_SUBS_OCTET  = 0xDD;

static constexpr int MIN_FRAME_SIZE = sizeof(quint8) + sizeof(quint16) + 1;

static const quint16 _crc_ccitt_lut[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108,
    0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210,
    0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
    0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
    0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee,
    0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6,
    0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d,
    0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5,
    0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc,
    0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4,
    0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
    0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13,
    0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a,
    0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e,
    0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1,
    0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb,
    0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
    0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
    0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657,
    0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9,
    0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882,
    0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e,
    0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07,
    0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d,
    0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
    0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

static inline void _encodeSymbol(QByteArray &buffer, char ch) Q_DECL_NOTHROW
{
    switch (ch)
    {
        case END_SLIP_OCTET:
            buffer.append(ESC_SLIP_OCTET);
            buffer.append(END_SUBS_OCTET);
            break;

        case ESC_SLIP_OCTET:
            buffer.append(ESC_SLIP_OCTET);
            buffer.append(ESC_SUBS_OCTET);
            break;

        default:
            buffer.append(ch);
    }
}

TestClient::TestClient(QSettings *settings, SessionManager *session, int no, QObject *parent)
    : QObject(parent),
      _no(no),
      _settings(settings),
      _session(session),
      _serial(this)
{
    connect(&_serial, &QSerialPort::readyRead, this, &TestClient::onSerialPortReadyRead);
    connect(&_serial, &QSerialPort::errorOccurred, this, &TestClient::onSerialPortErrorOccurred);

    connect(this, &TestClient::open, this, &TestClient::on_open);

    connect(this, &TestClient::checkDutsCurrent, [this](){_mode = slipMode;});
    connect(this, &TestClient::checkDutsCurrent, this, &TestClient::on_checkDutsCurrent);

    connect(this, &TestClient::startTesting, this, &TestClient::on_startTesting);

    connect(this, &TestClient::delay, this, &TestClient::on_delay);
    connect(this, &TestClient::waitCommandFinished, this, &TestClient::on_waitCommandFinished);

    connect(this, &TestClient::addJlinkToSriptEngine, this, &TestClient::on_addJlinkToSriptEngine);

    connect(this, &TestClient::setDutProperty, this, &TestClient::on_setDutProperty);

    //Slip commands

    connect(this, &TestClient::sendRailtestCommand, this, &TestClient::on_sendRailtestCommand);
    connect(this, &TestClient::reset, this, &TestClient::on_reset);

    connect(this, &TestClient::switchSWD, [this](){_mode = slipMode;});
    connect(this, &TestClient::switchSWD, this, &TestClient::on_switchSWD);

    connect(this, &TestClient::powerOn, [this](){_mode = slipMode;});
    connect(this, &TestClient::powerOn, this, &TestClient::on_powerOn);

    connect(this, &TestClient::powerOff, [this](){_mode = slipMode;});
    connect(this, &TestClient::powerOff, this, &TestClient::on_powerOff);

    connect(this, &TestClient::readDIN, this, &TestClient::on_readDIN);
    connect(this, &TestClient::setDOUT, this, &TestClient::on_setDOUT);
    connect(this, &TestClient::clearDOUT, this, &TestClient::on_clearDOUT);

    connect(this, &TestClient::readCSA, [this](){_mode = slipMode;});
    connect(this, &TestClient::readCSA, this, &TestClient::on_readCSA);

    connect(this, &TestClient::readAIN, [this](){_mode = slipMode;});
    connect(this, &TestClient::readAIN, this, &TestClient::on_readAIN);

    connect(this, &TestClient::daliOn, [this](){_mode = slipMode;});
    connect(this, &TestClient::daliOn, this, &TestClient::on_DaliOn);

    connect(this, &TestClient::daliOff, [this](){_mode = slipMode;});
    connect(this, &TestClient::daliOff, this, &TestClient::on_DaliOff);

    connect(this, &TestClient::configDebugSerial, this, &TestClient::on_configDebugSerial);
    connect(this, &TestClient::readDaliADC, this, &TestClient::on_readDaliADC);
    connect(this, &TestClient::readDinADC, this, &TestClient::on_readDinADC);
    connect(this, &TestClient::read24V, this, &TestClient::on_read24V);
    connect(this, &TestClient::read3V, this, &TestClient::on_read3V);
    connect(this, &TestClient::readTemperature, this, &TestClient::on_readTemperature);

    //Rail test commands

//    connect(this, &TestClient::waitCommandPrompt, [this](){_mode = railMode;});
//    connect(this, &TestClient::waitCommandPrompt, &_rail, &RailtestClient::on_waitCommandPrompt);

//    connect(this, &TestClient::syncCommand, &_rail, &RailtestClient::on_syncCommand);

    connect(this, &TestClient::readChipId, [this](){_mode = railMode;});
    connect(this, &TestClient::readChipId, this, &TestClient::on_readChipId);

    connect(this, &TestClient::testAccelerometer, [this](){_mode = railMode;});
    connect(this, &TestClient::testAccelerometer, this, &TestClient::on_testAccelerometer);

    connect(this, &TestClient::testLightSensor, [this](){_mode = railMode;});
    connect(this, &TestClient::testLightSensor, this, &TestClient::on_testLightSensor);

    connect(this, &TestClient::testDALI, [this](){_mode = railMode;});
    connect(this, &TestClient::testDALI, this, &TestClient::on_testDALI);

//    connect(this, &TestClient::testRadio, &_rail, &RailtestClient::on_testRadio);
//    connect(this, &TestClient::testGNSS, &_rail, &RailtestClient::on_testGNSS);

    _duts[1] = dutTemplate;
    _duts[2] = dutTemplate;
    _duts[3] = dutTemplate;
}

TestClient::~TestClient()
{

}

void TestClient::initTestMethodManager()
{
    _methodManager = new TestMethodManager(_settings);
    _methodManager->moveToThread(thread());
    _methodManager->setLogger(_logger);
    _methodManager->getScriptEngine()->globalObject().setProperty("testClient", _methodManager->getScriptEngine()->newQObject(this));
    connect(this, &TestClient::runTestFunction, _methodManager, &TestMethodManager::runTestFunction);
}

void TestClient::setLogger(Logger *logger)
{
    _logger = logger;
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

void TestClient::setPort(const QString &name, qint32 baudRate, QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity, QSerialPort::StopBits stopBits,
    QSerialPort::FlowControl flowControl)
{
    _serial.setPortName(name);

    bool res = _serial.setBaudRate(baudRate)
        && _serial.setDataBits(dataBits)
        && _serial.setParity(parity)
        && _serial.setStopBits(stopBits)
        && _serial.setFlowControl(flowControl);

    if (!res)
        qDebug() << _serial.errorString();
}

void TestClient::on_open()
{
    if (_serial.isOpen())
    {
        _serial.close();
    }

    if (!_serial.open(QSerialPort::ReadWrite))
        qDebug() << _serial.errorString();
    else
    {
        _serial.readAll();
        readCSA(0);
    }
}

void TestClient::close()
{
    if (_serial.isOpen())
    {
        _serial.flush();
        _serial.close();
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

void TestClient::checkTestingCompletion()
{
    for(auto & dut : _duts)
    {
        if(dut["checked"].toBool())
        {
            bool result =   dut["id"].toString() != "" &&
                            dut["voltageChecked"].toBool() &&
                            dut["lightSensChecked"].toBool() &&
                            dut["daliChecked"].toBool() &&
                            dut["accelChecked"].toBool();

            if(result)
            {
                dut["state"] = DutState::tested;
            }

            else
            {
                dut["state"] = DutState::warning;
            }

            emit dutChanged(dut);
            emit dutFullyTested(dut);
        }
    }
}

void TestClient::onSerialPortReadyRead()
{
    switch (_mode)
    {
    case idleMode:
        _serial.readAll();
        break;

    case slipMode:
    case railMode:
        processResponsePacket();
        break;
    }
}

void TestClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode)
{
    if (errorCode != QSerialPort::NoError)
        _logger->logError("Serial port error occurred " + QString().setNum(errorCode) + " on port " + _serial.portName());
}

void TestClient::on_sendRailtestCommand(int channel, const QByteArray &cmd, const QByteArray &args)
{
    _mode = railMode;
    _currentSlot = channel;
    _syncCommand = cmd;
    _syncReplies.clear();
    sendFrame(channel, cmd + " " + args + "\r\n\r\n");
}

void TestClient::on_reset()
{
    MB_Packet_t pkt;

    pkt.type = qToBigEndian<uint16_t>(MB_SYSTEM_RESET);
    pkt.sequence = MB_SYSTEM_RESET;
    pkt.dataLen = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_switchSWD(int DUT)
{
    _currentSlot = DUT;
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
    pkt.dut = DUT;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_powerOn(int DUT)
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
    pkt.dut = DUT;
    pkt.state = 1;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_powerOff(int DUT)
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
    pkt.dut = DUT;
    pkt.state = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readDIN(int DUT, int DIN)
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
    pkt.h.sequence = 4;
    pkt.h.dataLen = 2;
    pkt.dut = DUT;
    pkt.din = DIN;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_setDOUT(int DUT, int DOUT)
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
    pkt.h.sequence = 5;
    pkt.h.dataLen = 3;
    pkt.dut = DUT;
    pkt.dout = DOUT;
    pkt.state = 1;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_clearDOUT(int DUT, int DOUT)
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
    pkt.h.sequence = 6;
    pkt.h.dataLen = 3;
    pkt.dut = DUT;
    pkt.dout = DOUT;
    pkt.state = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readCSA(int gain)
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
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readAIN(int DUT, int AIN, int gain)
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
    pkt.dut = DUT;
    pkt.ain = AIN;
    pkt.gain = gain;

    _currentSlot = DUT;

    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_configDebugSerial(int DUT, int baudRate, unsigned char bits, unsigned char parity, unsigned char stopBits)
{
    MB_ConfigDutDebug_t pkt;

    pkt.header.type = qToBigEndian<uint16_t>(MB_CONFIG_DUT_DEBUG);
    pkt.header.sequence = 9;
    pkt.header.dataLen = sizeof(MB_ConfigDutDebug_t) - sizeof(MB_Packet_t);
    pkt.dutIndex = DUT;
    pkt.baudRate = qToBigEndian(baudRate);
    pkt.bits = bits;
    pkt.parity = parity;
    pkt.stopBits = stopBits;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_DaliOn()
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
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_DaliOff()
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
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readDaliADC()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DALI_ADC);
    pkt.h.sequence = 12;
    pkt.h.dataLen = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readDinADC(int DUT, int DIN)
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
    pkt.h.sequence = 13;
    pkt.h.dataLen = 2;
    pkt.dut = DUT;
    pkt.din = DIN;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_read24V()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_24V);
    pkt.h.sequence = 14;
    pkt.h.dataLen = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_read3V()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_3V);
    pkt.h.sequence = 15;
    pkt.h.dataLen = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readTemperature()
{
#pragma pack (push, 1)
    struct Pkt
    {
        MB_Packet_t h;
    };
#pragma pack (pop)

    Pkt pkt;

    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_TEMP);
    pkt.h.sequence = 16;
    pkt.h.dataLen = 0;
    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

void TestClient::on_readChipId(int slot)
{
    _currentCommand = readChipIdCommand;
    sendRailtestCommand(slot, "getmemw", "0x0FE081F0 2");
//    delay(1000);
    _duts[slot]["id"] = _currentChipID;
    emit dutChanged(_duts[slot]);
    _logger->logSuccess(QString("ID for DUT %1 has been read: %2").arg(_duts[slot]["no"].toInt()).arg(_duts[slot]["id"].toString()));
}

void TestClient::on_testRadio(int slot)
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

    on_sendRailtestCommand(slot, "rx", "0");
    on_delay(1000);

    on_sendRailtestCommand(slot, "setBleMode", "1");
    on_delay(1000);

    on_sendRailtestCommand(slot, "setBle1Mbps", "1");
    on_delay(1000);

    on_sendRailtestCommand(slot, "setChannel", "19");
    on_delay(1000);

    on_sendRailtestCommand(slot, "setPower", "80");
    on_delay(1000);

    rf.syncCommand("rx", "1", 1000);

    on_sendRailtestCommand(slot, "tx", "11");
    on_delay(5000);

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

void TestClient::on_testAccelerometer(int slot)
{
    _currentCommand = accelCommand;
    sendRailtestCommand(slot, "accl", {});
//    delay(1000);
    _duts[slot]["accelChecked"] = _currentAccelChecked;

    if(_duts[slot]["accelChecked"].toBool())
        _logger->logSuccess(QString("Accelerometer in DUT %1 has been tested successfully").arg(_duts[slot]["no"].toInt()));
    else
    {
        _logger->logError(QString("Testing accelerometer in DUT %1 has been failed!").arg(_duts[slot]["no"].toInt()));
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + _currentError;
    }

    emit dutChanged(_duts[slot]);
}

void TestClient::on_testLightSensor(int slot)
{
    _currentCommand = lightSensCommand;
    sendRailtestCommand(slot, "lsen", {});
//    delay(1000);
    _duts[slot]["lightSensChecked"] = _currentLightSensChecked;

    if(_duts[slot]["lightSensChecked"].toBool())
        _logger->logSuccess(QString("Light sensor in DUT %1 has been tested successfully").arg(_duts[slot]["no"].toInt()));
    else
    {
        _logger->logError(QString("Testing light sensor in DUT %1 has been failed!").arg(_duts[slot]["no"].toInt()));
        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + _currentError;
    }

    emit dutChanged(_duts[slot]);
}

void TestClient::on_testDALI()
{
    sendRailtestCommand(_currentSlot, "dali", {"0xFE80 16 0 0"});
    delay(2000);

    _currentCommand = daliCommand;
    sendRailtestCommand(_currentSlot, "dali", {"0xFF90 16 0 1000000"});
    delay(2500);
    _duts[_currentSlot]["daliChecked"] = _currentDaliChecked;

    if(_duts[_currentSlot]["daliChecked"].toBool())
        _logger->logSuccess(QString("DALI for DUT %1 has been tested successfully").arg(_duts[_currentSlot]["no"].toInt()));
    else
    {
        _logger->logError(QString("Testing DALI for DUT %1 has been failed!").arg(_duts[_currentSlot]["no"].toInt()));
        _duts[_currentSlot]["error"] = _duts[_currentSlot]["error"].toString() + "; " + _currentError;
    }

    sendRailtestCommand(_currentSlot, "dali", {"0xFE80 16 0 0"});
    delay(2000);

    emit dutChanged(_duts[_currentSlot]);
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

void TestClient::sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW
{
    QByteArray encodedBuffer;
    quint16 frameCrc = 0xFFFF;

    // Write UART wake up symbols and SLIP frame start.
    encodedBuffer.reserve((frame.size() + 3) * 2 + 2);
    encodedBuffer.append(END_SLIP_OCTET);

    // Write escaped channel number.
    quint8 index = channel ^ (frameCrc >> 8);

    _encodeSymbol(encodedBuffer, channel);
    frameCrc = _crc_ccitt_lut[index] ^ (frameCrc << 8);

    // Write escaped frame and calculate CRC.
    foreach (char ch, frame)
    {
        quint8 index = (quint8)ch ^ (frameCrc >> 8);

        _encodeSymbol(encodedBuffer, ch);
        frameCrc = _crc_ccitt_lut[index] ^ (frameCrc << 8);
    }

    // Write escaped CRC.
    _encodeSymbol(encodedBuffer, frameCrc >> 8);
    _encodeSymbol(encodedBuffer, frameCrc & 0xFF);

    // Write SLIP frame end.
    encodedBuffer.append(END_SLIP_OCTET);

    // Write encoded frame to serial port.
    _serial.write(encodedBuffer);
    on_waitCommandFinished();
}

void TestClient::processResponsePacket()
{
    while (_serial.bytesAvailable())
    {
        QByteArray buffer = _serial.readAll();
        foreach (char ch, buffer)
        {
            if (ch == END_SLIP_OCTET)
                if (_frameStarted)
                {
                    if (_recvBuffer.size() >= MIN_FRAME_SIZE)
                    {
                        decodeFrame();
                        _frameStarted = false;
                    }
                    _recvBuffer.clear();
                }
                else
                {
                    _frameStarted = true;
                    _recvBuffer.clear();
                }
            else
                if (_frameStarted)
                    _recvBuffer.append(ch);
        }
    }
}

void TestClient::decodeFrame() Q_DECL_NOTHROW
{
    QByteArray decodedBuffer;
    static QByteArray railReply;

    // Write unescaped buffer.
    decodedBuffer.reserve(_recvBuffer.size());
    for (int i = 0; i < _recvBuffer.size(); ++i)
    {
        char ch = _recvBuffer.at(i);

        if (ch == ESC_SLIP_OCTET)
        {
            ++i;
            if (i >= _recvBuffer.size())
            {
                _logger->logError("SLIP. Decode frame. Unfinished escape sequence.");
                return;
            }
            switch (_recvBuffer.at(i))
            {
                case END_SUBS_OCTET:
                    decodedBuffer.append(END_SLIP_OCTET);
                    break;

                case ESC_SUBS_OCTET:
                    decodedBuffer.append(ESC_SLIP_OCTET);
                    break;

                default:
                    _logger->logError("SLIP. Decode frame. Invalid escape sequence.");
                    return;
            }
        }
        else
            decodedBuffer.append(ch);
    }
    if (decodedBuffer.size() < MIN_FRAME_SIZE)
    {
        _logger->logError("SLIP. Decode frame. Frame too short.");
        return;
    }

    // Calculate CRC.
    int frameSize = decodedBuffer.size() - 2;
    quint16
        frameCrc = 0xFFFF,
        bufferCrc = (quint8)decodedBuffer.at(frameSize + 1) | ((quint16)decodedBuffer.at(frameSize) << 8);

    for (int i = 0; i < frameSize; ++i)
    {
        quint8 index = (quint8)decodedBuffer.at(i) ^ (frameCrc >> 8);

        frameCrc = _crc_ccitt_lut[index] ^ (frameCrc << 8);
    }

    // Check CRC.
    if (frameCrc != bufferCrc)
    {
        _logger->logError("SLIP. Decode frame. Invalid Frame CRC.");
        return;
    }

    // Frame received successfully.

    QByteArray frame = decodedBuffer.mid(1, frameSize - 1);

    switch(_mode)
    {
    case slipMode:
        onSlipPacketReceived(decodedBuffer.at(0), frame);
        break;

    case railMode:
        railReply += frame;
        if(frame.contains("> "))
        {
            onSlipPacketReceived(decodedBuffer.at(0), railReply);
            railReply.clear();
        }
        break;
    }
}

void TestClient::onSlipPacketReceived(quint8 channel, QByteArray frame) noexcept
{
    switch (channel)
    {
        case 0:
            if (frame.size() >= (int)sizeof(MB_Packet_t))
            {
                MB_Packet_t *pkt = (MB_Packet_t*)frame.data();

                pkt->type = qFromBigEndian(pkt->type);
                switch (pkt->type)
                {
                    case MB_STARTUP:
                        _logger->logInfo("Startup event.");
                        break;

                    case MB_GENERAL_RESULT:
                        {
                            MB_GeneralResult_t *gr = (MB_GeneralResult_t*)pkt;

                            gr->errorCode = qFromBigEndian(gr->errorCode);
                            switch (gr->header.sequence)
                            {
                            case 1:
                                //_logger->logDebug(QString("Reply to switchSWD command to board on %1: %2").arg(_serial.portName()).arg(gr->errorCode));
                                break;

                            case 2:
                                //_logger->logDebug(QString("Reply to powerOn command to %1: %2").arg(_serial.portName()).arg(gr->errorCode));
                                break;

                            case 3:
                                //_logger->logDebug(QString("Reply to powerOff command to %1: %2").arg(_serial.portName()).arg(gr->errorCode));
                                break;

                            case 7:
                                _CSA = gr->errorCode;
                                //_logger->logDebug(QString("Reply to readCSA command to measuring board %1: %2").arg(_no).arg(gr->errorCode));
                                break;

                            case 8:
                                _currentVoltage = gr->errorCode;

                                if(_currentVoltage > 70000 && _currentVoltage < 72000)
                                {
                                    _duts[_currentSlot]["voltageChecked"] = true;
                                    _logger->logSuccess(QString("Voltage (3.3V) on AIN 1 in DUT %1 checked").arg(_duts[_currentSlot]["no"].toInt()));
                                }
                                else
                                {
                                    _duts[_currentSlot]["voltageChecked"] = false;
                                    _duts[_currentSlot]["error"] = _duts[_currentSlot]["error"].toString() + "Error voltage value on AIN 1. " + QString("Code:%1").arg(gr->errorCode) + "; ";
                                    _logger->logError(QString("Error voltage value on AIN 1 in DUT %1 detected!").arg(_duts[_currentSlot]["no"].toInt()));
                                    _logger->logError(QString("Code:%1").arg(gr->errorCode));
                                }

                                emit dutChanged(_duts[_currentSlot]);
                                _currentVoltage = 0;
                                break;
                            }
                        }
                        break;

                    case MB_ASYNC_EVENT:
                        {
                            MB_Event_t *evt = (MB_Event_t*)pkt;

                            evt->eventCode = qFromBigEndian(evt->eventCode);
                            _logger->logInfo(QString("EVENT: code=%2.").arg(evt->eventCode));
                        }
                        break;
                }
            }
            break;

        case 1:
        case 2:
        case 3:
            processFrameFromRail(frame);
            break;
    }

    _mode = idleMode;
}

void TestClient::decodeRailtestReply(const QByteArray &reply)
{
    if (reply.startsWith("{{(") && reply.endsWith("}}"))
    {
        int idx = reply.indexOf(")}");

        if (idx == -1)
            return;

        auto name = reply.mid(3, idx - 3);
        auto params = reply.mid(idx + 2, reply.length() - idx - 3).split('}');
        QVariantMap decodedParams;

        foreach (auto param, params)
            if (param.startsWith('{'))
            {
                idx = param.indexOf(':');
                if (idx != -1)
                    decodedParams.insert(param.mid(1, idx - 1), param.mid(idx + 1));
            }

        if (_syncCommand == name)
            _syncReplies.append(decodedParams);

        return;
    }

    if (reply.startsWith("{{") && reply.endsWith("}}"))
    {
        auto params = reply.mid(1, reply.length() - 2).split('}');
        QVariantList decodedParams;

        foreach (auto param, params)
            if (param.startsWith('{'))
                decodedParams.append(param.mid(1));

        if (!_syncCommand.isEmpty())
            _syncReplies.push_back(decodedParams);

        return;
    }
}

void TestClient::processFrameFromRail(QByteArray frame)
{
    switch (_currentCommand)
    {
    case readChipIdCommand:
    {
        //------------------------------------------------------------
        int idx = frame.indexOf("\r\n");
        frame = frame.mid(idx + 2); // Delete first line
        frame = frame.mid(1); // Delete '#' symbol

        QString frameString(frame);

        auto replyStringList = frameString.split("\r\n");
        QList<QByteArray> replyList;
        for (int i = 1; i < replyStringList.size() - 2; i++)
        {
            replyList.push_back(replyStringList.at(i).toLocal8Bit());
        }

        for(auto & reply : replyList)
        {
            decodeRailtestReply(reply);
        }
        //-------------------------------------------------------------

        auto llo = _syncReplies.at(0).toList();
        auto lhi = _syncReplies.at(1).toList();

        auto lo = llo.at(1).toByteArray().trimmed();
        auto hi = lhi.at(1).toByteArray().trimmed();

        _currentChipID = (hi.mid(2) + lo.mid(2)).toUpper();
    }
        break;

    case accelCommand:
    {
        if (frame.contains("X") && frame.contains("Y") && frame.contains("Z"))
        {
               double x = frame.mid(frame.indexOf("X") + 2, 5).toDouble();
               double y = frame.mid(frame.indexOf("Y") + 2, 5).toDouble();
               double z = frame.mid(frame.indexOf("Z") + 2, 5).toDouble();

               if (x > 10 || x < -10 || y > 10 || y < -10 || z < 80 || z > 100)
               {
                   _logger->logError(QString("Accelerometer failure: X=%1, Y=%2, Z=%3.").arg(x).arg(y).arg(z));
                   _currentAccelChecked = false;
                   _currentError = QString("Accelerometer failure: X=%1, Y=%2, Z=%3.").arg(x).arg(y).arg(z);
               }
               else
               {
                   //_logger->logSuccess(QString("Accelerometer: X=%1, Y=%2, Z=%3.").arg(x).arg(y).arg(z));
                   _currentAccelChecked = true;
               }
        }

        else
        {
            _currentAccelChecked = false;
        }
    }
        break;

    case lightSensCommand:
    {
        if (frame.contains("opwr"))
        {
               double opwr = frame.mid(frame.indexOf("opwr") + 5, 5).toDouble();

               if (opwr < 0)
               {
                   _logger->logError(QString("Light sensor failure: opwr=%1.").arg(opwr));
                   _currentLightSensChecked = false;
                   _currentError = QString("Light sensor failure: opwr=%1.").arg(opwr);
               }
               else
               {
                   _currentLightSensChecked = true;
               }
        }

        else
        {
            _currentAccelChecked = false;
        }
    }
        break;

    case daliCommand:
    {
        if (frame.contains("error"))
        {
               int code = frame.mid(frame.indexOf("error") + 6, 1).toInt();
               if (code != 0)
               {
                   _logger->logError(frame);
                   _currentDaliChecked = false;
                   _currentError = frame;
               }
               else
               {
                   _currentDaliChecked = true;
               }
        }

        else
        {
            _currentDaliChecked = false;
        }
    }
        break;
    }

    _currentCommand = noCommand;
}

void TestClient::on_checkDutsCurrent()
{
    _isActive = false;

    for (int slot = 1; slot < _duts.size() + 1; slot++)
    {
        powerOff(slot);
    }

    delay(2000);

    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        readCSA(0);
        delay(100);
        int currentCSA = _CSA;

        powerOn(slot);
        delay(100);

        readCSA(0);
        delay(100);
        if((_CSA - currentCSA) > 15 && currentCSA != -1)
        {
            _logger->logSuccess(QString("Device connected to the slot %1 of the test board on port %2").arg(slot).arg(_serial.portName()));
            _duts[slot]["state"] = DutState::untested;
            _duts[slot]["checked"] = true;
            _isActive = true;
        }

        else
        {
            _duts[slot]["state"] = DutState::inactive;
            _duts[slot]["checked"] = false;
        }

        emit dutChanged(_duts[slot]);
        powerOff(slot);
        delay(2000);
    }

    emit commandSequenceFinished();
}

void TestClient::on_startTesting()
{
    if(!isActive())
    {
        emit commandSequenceFinished();
        return;
    }

    //Supply power to all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        powerOn(slot);
    }

    delay(1000);

    //Read ID for all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            readChipId(slot);
        }
    }

    //Read AIN1 for all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            readAIN(slot, 1, 0);
        }
    }

    //Test accelerometer for all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            testAccelerometer(slot);
        }
    }

    //Test light sensor for all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            testLightSensor(slot);
        }
    }

    //PowerOff all DUTs
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        powerOff(slot);
    }

    //Test DALI interface for all DUTs
    daliOn();
    for(int slot = 1; slot < _duts.size() + 1; slot++)
    {
        if(isDutAvailable(slot) && isDutChecked(slot))
        {
            switchSWD(slot);
            powerOn(slot);
            delay(1000);

            testDALI();
            delay(500);

            powerOff(slot);
        }
    }
    daliOff();

    checkTestingCompletion();

    emit commandSequenceFinished();
}

void TestClient::on_delay(int msec)
{
    QTime expire = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
    }
}

void TestClient::on_waitCommandFinished()
{
    while(_mode != idleMode)
    {
        QCoreApplication::processEvents();
    }
}

void TestClient::on_addJlinkToSriptEngine()
{
    _methodManager->getScriptEngine()->globalObject().setProperty("jlink", _methodManager->getScriptEngine()->newQObject(_jlinkManager));
}

void TestClient::on_setDutProperty(int slot, const QString &property, const QVariant &value)
{
    _duts[slot][property] = value;
    emit dutChanged(_duts[slot]);
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
