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



//    connect(this, &TestClient::open, this, &TestClient::on_open);

//    connect(this, &TestClient::checkDutsCurrent, [this](){_mode = slipMode;});
//    connect(this, &TestClient::checkDutsCurrent, this, &TestClient::on_checkDutsCurrent);

//    connect(this, &TestClient::startTesting, this, &TestClient::on_startTesting);

//    connect(this, &TestClient::call_delay, this, &TestClient::on_delay);
//    connect(this, &TestClient::waitCommandFinished, this, &TestClient::on_waitCommandFinished);

//    connect(this, &TestClient::addJlinkToSriptEngine, this, &TestClient::on_addJlinkToSriptEngine);

    connect(this, &TestClient::setDutProperty, this, &TestClient::on_setDutProperty);
    connect(this, &TestClient::resetDut, this, &TestClient::on_resetDut);

    //Slip commands

//    connect(this, &TestClient::call_sendRailtestCommand, this, &TestClient::on_sendRailtestCommand);
//    connect(this, &TestClient::call_reset, this, &TestClient::on_reset);

//    connect(this, &TestClient::call_switchSWD, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_switchSWD, this, &TestClient::on_switchSWD);

//    connect(this, &TestClient::call_powerOn, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_powerOn, this, &TestClient::on_powerOn);

//    connect(this, &TestClient::call_powerOff, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_powerOff, this, &TestClient::on_powerOff);

//    connect(this, &TestClient::call_readDIN, this, &TestClient::on_readDIN);
//    connect(this, &TestClient::call_setDOUT, this, &TestClient::on_setDOUT);
//    connect(this, &TestClient::call_clearDOUT, this, &TestClient::on_clearDOUT);

//    connect(this, &TestClient::call_readCSA, [this](){_mode = slipMode;});
    connect(this, &TestClient::call_readCSA, this, &TestClient::readCSA);

//    connect(this, &TestClient::call_readAIN, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_readAIN, this, &TestClient::on_readAIN);

//    connect(this, &TestClient::call_daliOn, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_daliOn, this, &TestClient::on_DaliOn);

//    connect(this, &TestClient::call_daliOff, [this](){_mode = slipMode;});
//    connect(this, &TestClient::call_daliOff, this, &TestClient::on_DaliOff);

//    connect(this, &TestClient::configDebugSerial, this, &TestClient::on_configDebugSerial);
//    connect(this, &TestClient::call_readDaliADC, this, &TestClient::on_readDaliADC);
//    connect(this, &TestClient::call_readDinADC, this, &TestClient::on_readDinADC);
//    connect(this, &TestClient::call_read24V, this, &TestClient::on_read24V);
//    connect(this, &TestClient::call_read3V, this, &TestClient::on_read3V);
//    connect(this, &TestClient::call_readTemperature, this, &TestClient::on_readTemperature);

    //Rail test commands

//    connect(this, &TestClient::waitCommandPrompt, [this](){_mode = railMode;});
//    connect(this, &TestClient::waitCommandPrompt, &_rail, &RailtestClient::on_waitCommandPrompt);

//    connect(this, &TestClient::syncCommand, &_rail, &RailtestClient::on_syncCommand);

//    connect(this, &TestClient::readChipId, [this](){_mode = railMode;});
//    connect(this, &TestClient::readChipId, this, &TestClient::on_readChipId);

//    connect(this, &TestClient::testAccelerometer, [this](){_mode = railMode;});
//    connect(this, &TestClient::testAccelerometer, this, &TestClient::on_testAccelerometer);

//    connect(this, &TestClient::testLightSensor, [this](){_mode = railMode;});
//    connect(this, &TestClient::testLightSensor, this, &TestClient::on_testLightSensor);

//    connect(this, &TestClient::testDALI, [this](){_mode = railMode;});
//    connect(this, &TestClient::testDALI, this, &TestClient::on_testDALI);

//    connect(this, &TestClient::testRadio, &_rail, &RailtestClient::on_testRadio);
//    connect(this, &TestClient::testGNSS, &_rail, &RailtestClient::on_testGNSS);

    _duts[1] = dutTemplate;
    _duts[2] = dutTemplate;
    _duts[3] = dutTemplate;
}

TestClient::~TestClient()
{

}

//void TestClient::initTestMethodManager()
//{
//    _methodManager = new TestMethodManager(_settings);
//    _methodManager->moveToThread(thread());
//    _methodManager->setLogger(_logger);
//    _methodManager->getScriptEngine()->globalObject().setProperty("testClient", _methodManager->getScriptEngine()->newQObject(this));
//    connect(this, &TestClient::runTestFunction, _methodManager, &TestMethodManager::runTestFunction);
//}

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

//void TestClient::checkTestingCompletion()
//{
//    for(auto & dut : _duts)
//    {
//        if(dut["checked"].toBool())
//        {
//            bool result =   dut["id"].toString() != "" &&
//                            dut["voltageChecked"].toBool() &&
//                            dut["lightSensChecked"].toBool() &&
//                            dut["daliChecked"].toBool() &&
//                            dut["accelChecked"].toBool();

//            if(result)
//            {
//                dut["state"] = DutState::tested;
//            }

//            else
//            {
//                dut["state"] = DutState::warning;
//            }

//            emit dutChanged(dut);
//            emit dutFullyTested(dut);
//        }
//    }
//}

//void TestClient::on_sendRailtestCommand(int channel, const QByteArray &cmd, const QByteArray &args)
//{
//    _mode = railMode;
//    _currentSlot = channel;
//    _syncCommand = cmd;
//    _syncReplies.clear();
//    sendFrame(channel, cmd + " " + args + "\r\n\r\n");
//}

//void TestClient::on_reset()
//{
//    MB_Packet_t pkt;

//    pkt.type = qToBigEndian<uint16_t>(MB_SYSTEM_RESET);
//    pkt.sequence = MB_SYSTEM_RESET;
//    pkt.dataLen = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_switchSWD(int DUT)
//{
//    _currentSlot = DUT;
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t dut;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_SWD);
//    pkt.h.sequence = 1;
//    pkt.h.dataLen = 1;
//    pkt.dut = DUT;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_powerOn(int DUT)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_POWER);
//    pkt.h.sequence = 2;
//    pkt.h.dataLen = 2;
//    pkt.dut = DUT;
//    pkt.state = 1;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_powerOff(int DUT)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_POWER);
//    pkt.h.sequence = 3;
//    pkt.h.dataLen = 2;
//    pkt.dut = DUT;
//    pkt.state = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_readDIN(int DUT, int DIN)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                din;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DIN);
//    pkt.h.sequence = 4;
//    pkt.h.dataLen = 2;
//    pkt.dut = DUT;
//    pkt.din = DIN;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_setDOUT(int DUT, int DOUT)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                dout,
//                state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_WRITE_DOUT);
//    pkt.h.sequence = 5;
//    pkt.h.dataLen = 3;
//    pkt.dut = DUT;
//    pkt.dout = DOUT;
//    pkt.state = 1;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_clearDOUT(int DUT, int DOUT)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                dout,
//                state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_WRITE_DOUT);
//    pkt.h.sequence = 6;
//    pkt.h.dataLen = 3;
//    pkt.dut = DUT;
//    pkt.dout = DOUT;
//    pkt.state = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

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
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
}

//void TestClient::on_readAIN(int DUT, int AIN, int gain)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                ain,
//                gain;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ANALOG);
//    pkt.h.sequence = 8;
//    pkt.h.dataLen = 3;
//    pkt.dut = DUT;
//    pkt.ain = AIN;
//    pkt.gain = gain;

//    _currentSlot = DUT;

//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_configDebugSerial(int DUT, int baudRate, unsigned char bits, unsigned char parity, unsigned char stopBits)
//{
//    MB_ConfigDutDebug_t pkt;

//    pkt.header.type = qToBigEndian<uint16_t>(MB_CONFIG_DUT_DEBUG);
//    pkt.header.sequence = 9;
//    pkt.header.dataLen = sizeof(MB_ConfigDutDebug_t) - sizeof(MB_Packet_t);
//    pkt.dutIndex = DUT;
//    pkt.baudRate = qToBigEndian(baudRate);
//    pkt.bits = bits;
//    pkt.parity = parity;
//    pkt.stopBits = stopBits;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_DaliOn()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_DALI);
//    pkt.h.sequence = 10;
//    pkt.h.dataLen = 1;
//    pkt.state = 1;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_DaliOff()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t state;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_SWITCH_DALI);
//    pkt.h.sequence = 11;
//    pkt.h.dataLen = 1;
//    pkt.state = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_readDaliADC()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DALI_ADC);
//    pkt.h.sequence = 12;
//    pkt.h.dataLen = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_readDinADC(int DUT, int DIN)
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//        uint8_t
//                dut,
//                din;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_DIN_ADC);
//    pkt.h.sequence = 13;
//    pkt.h.dataLen = 2;
//    pkt.dut = DUT;
//    pkt.din = DIN;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_read24V()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_24V);
//    pkt.h.sequence = 14;
//    pkt.h.dataLen = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_read3V()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_3V);
//    pkt.h.sequence = 15;
//    pkt.h.dataLen = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_readTemperature()
//{
//#pragma pack (push, 1)
//    struct Pkt
//    {
//        MB_Packet_t h;
//    };
//#pragma pack (pop)

//    Pkt pkt;

//    pkt.h.type = qToBigEndian<uint16_t>(MB_READ_ADC_TEMP);
//    pkt.h.sequence = 16;
//    pkt.h.dataLen = 0;
//    sendFrame(0, QByteArray((char*)&pkt, sizeof(pkt)));
//}

//void TestClient::on_readChipId(int slot)
//{
//    _currentCommand = readChipIdCommand;
//    call_sendRailtestCommand(slot, "getmemw", "0x0FE081F0 2");
////    delay(1000);
//    _duts[slot]["id"] = _currentChipID;
//    emit dutChanged(_duts[slot]);
//    _logger->logSuccess(QString("ID for DUT %1 has been read: %2").arg(_duts[slot]["no"].toInt()).arg(_duts[slot]["id"].toString()));
//}

//void TestClient::on_testRadio(int slot)
//{
//    RailtestClient rf;

//    connect(&rf, &RailtestClient::replyReceived, this, &TestClient::onRfReplyReceived);

//    if (!rf.open(_settings->value("Railtest/rf_serial").toString()))
//    {
//        _logger->logError("Cannot open serial port for reference radio module!");
//        return;
//    }

//    rf.syncCommand("reset", "", 3000);
//    if (!rf.waitCommandPrompt())
//    {
//        _logger->logError("Timeout waiting reference radio module command prompt!");
//        return;
//    }

//    rf.syncCommand("rx", "0", 1000);
//    _rfRSSI = 255;
//    _rfCount = 0;
//    rf.syncCommand("setBleMode", "1", 1000);
//    rf.syncCommand("setBle1Mbps", "1", 1000);
//    rf.syncCommand("setChannel", "19", 1000);

//    on_sendRailtestCommand(slot, "rx", "0");
//    on_delay(1000);

//    on_sendRailtestCommand(slot, "setBleMode", "1");
//    on_delay(1000);

//    on_sendRailtestCommand(slot, "setBle1Mbps", "1");
//    on_delay(1000);

//    on_sendRailtestCommand(slot, "setChannel", "19");
//    on_delay(1000);

//    on_sendRailtestCommand(slot, "setPower", "80");
//    on_delay(1000);

//    rf.syncCommand("rx", "1", 1000);

//    on_sendRailtestCommand(slot, "tx", "11");
//    on_delay(5000);

//    if (_rfCount < 8)
//    {
//        _logger->logError(QString("Radio Interface failure: packet lost (%1)!").arg(_rfCount));
//        _duts[slot]["radioChecked"] = false;
//        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: packet lost (%1)!").arg(_rfCount);
//    }

//    else if (_rfRSSI < -50 || _rfRSSI > 20)
//    {
//        _logger->logError(QString("Radio Interface failure: RSSI (%1) is out of bounds!").arg(_rfRSSI));
//        _duts[slot]["radioChecked"] = false;
//        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + QString("Radio Interface failure: RSSI (%1) is out of bounds!").arg(_rfRSSI);
//    }

//    else
//    {
//        _duts[slot]["radioChecked"] = true;
//    }
//}

//void TestClient::on_testAccelerometer(int slot)
//{
//    _currentCommand = accelCommand;
//    call_sendRailtestCommand(slot, "accl", {});
////    delay(1000);
//    _duts[slot]["accelChecked"] = _currentAccelChecked;

//    if(_duts[slot]["accelChecked"].toBool())
//        _logger->logSuccess(QString("Accelerometer in DUT %1 has been tested successfully").arg(_duts[slot]["no"].toInt()));
//    else
//    {
//        _logger->logError(QString("Testing accelerometer in DUT %1 has been failed!").arg(_duts[slot]["no"].toInt()));
//        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + _currentError;
//    }

//    emit dutChanged(_duts[slot]);
//}

//void TestClient::on_testLightSensor(int slot)
//{
//    _currentCommand = lightSensCommand;
//    call_sendRailtestCommand(slot, "lsen", {});
////    delay(1000);
//    _duts[slot]["lightSensChecked"] = _currentLightSensChecked;

//    if(_duts[slot]["lightSensChecked"].toBool())
//        _logger->logSuccess(QString("Light sensor in DUT %1 has been tested successfully").arg(_duts[slot]["no"].toInt()));
//    else
//    {
//        _logger->logError(QString("Testing light sensor in DUT %1 has been failed!").arg(_duts[slot]["no"].toInt()));
//        _duts[slot]["error"] = _duts[slot]["error"].toString() + "; " + _currentError;
//    }

//    emit dutChanged(_duts[slot]);
//}

//void TestClient::on_testDALI()
//{
//    call_sendRailtestCommand(_currentSlot, "dali", {"0xFE80 16 0 0"});
//    call_delay(2000);

//    _currentCommand = daliCommand;
//    call_sendRailtestCommand(_currentSlot, "dali", {"0xFF90 16 0 1000000"});
//    call_delay(2500);
//    _duts[_currentSlot]["daliChecked"] = _currentDaliChecked;

//    if(_duts[_currentSlot]["daliChecked"].toBool())
//        _logger->logSuccess(QString("DALI for DUT %1 has been tested successfully").arg(_duts[_currentSlot]["no"].toInt()));
//    else
//    {
//        _logger->logError(QString("Testing DALI for DUT %1 has been failed!").arg(_duts[_currentSlot]["no"].toInt()));
//        _duts[_currentSlot]["error"] = _duts[_currentSlot]["error"].toString() + "; " + _currentError;
//    }

//    call_sendRailtestCommand(_currentSlot, "dali", {"0xFE80 16 0 0"});
//    call_delay(2000);

//    emit dutChanged(_duts[_currentSlot]);
//}

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

//void TestClient::on_checkDutsCurrent()
//{
//    _isActive = false;

//    for (int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        call_powerOff(slot);
//    }

//    call_delay(2000);

//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        call_readCSA(0);
//        call_delay(100);
//        int currentCSA = _CSA;

//        call_powerOn(slot);
//        call_delay(100);

//        call_readCSA(0);
//        call_delay(100);
//        if((_CSA - currentCSA) > 15 && currentCSA != -1)
//        {
//            _logger->logSuccess(QString("Device connected to the slot %1 of the test board on port %2").arg(slot).arg(_serial.portName()));
//            _duts[slot]["state"] = DutState::untested;
//            _duts[slot]["checked"] = true;
//            _isActive = true;
//        }

//        else
//        {
//            _duts[slot]["state"] = DutState::inactive;
//            _duts[slot]["checked"] = false;
//        }

//        emit dutChanged(_duts[slot]);
//        call_powerOff(slot);
//        call_delay(2000);
//    }

//    emit commandSequenceFinished();
//}

//void TestClient::on_startTesting()
//{
//    if(!isActive())
//    {
//        emit commandSequenceFinished();
//        return;
//    }

//    //Supply power to all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        call_powerOn(slot);
//    }

//    call_delay(1000);

//    //Read ID for all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        if(isDutAvailable(slot) && isDutChecked(slot))
//        {
//            readChipId(slot);
//        }
//    }

//    //Read AIN1 for all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        if(isDutAvailable(slot) && isDutChecked(slot))
//        {
//            call_readAIN(slot, 1, 0);
//        }
//    }

//    //Test accelerometer for all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        if(isDutAvailable(slot) && isDutChecked(slot))
//        {
//            testAccelerometer(slot);
//        }
//    }

//    //Test light sensor for all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        if(isDutAvailable(slot) && isDutChecked(slot))
//        {
//            testLightSensor(slot);
//        }
//    }

//    //PowerOff all DUTs
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        call_powerOff(slot);
//    }

//    //Test DALI interface for all DUTs
//    call_daliOn();
//    for(int slot = 1; slot < _duts.size() + 1; slot++)
//    {
//        if(isDutAvailable(slot) && isDutChecked(slot))
//        {
//            call_switchSWD(slot);
//            call_powerOn(slot);
//            call_delay(1000);

//            testDALI();
//            call_delay(500);

//            call_powerOff(slot);
//        }
//    }
//    call_daliOff();

//    checkTestingCompletion();

//    emit commandSequenceFinished();
//}

void TestClient::on_delay(int msec)
{
    QTime expire = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
    }
}

//void TestClient::on_addJlinkToSriptEngine()
//{
//    _methodManager->getScriptEngine()->globalObject().setProperty("jlink", _methodManager->getScriptEngine()->newQObject(_jlinkManager));
//}

void TestClient::on_resetDut(int slot)
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
