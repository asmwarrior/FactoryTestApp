#include "TestClient.h"

#include <QTimer>

TestClient::TestClient(QSettings *settings, SessionManager *session, QObject *parent)
    : QObject(parent),
      _settings(settings),
      _session(session),
      _serial(this),
      _rail(_serial, settings, session, this),
      _slip(_serial, session, this)
{
    connect(&_serial, &QSerialPort::readyRead, this, &TestClient::onSerialPortReadyRead);
    connect(&_serial, &QSerialPort::errorOccurred, this, &TestClient::onSerialPortErrorOccurred);

    connect(&_slip, &SlipClient::commandFinished, [this](){_mode = idleMode;});

    connect(this, &TestClient::checkBoardCurrent, [this](){_mode = slipMode;});
    connect(this, &TestClient::checkBoardCurrent, &_slip, &SlipClient::on_checkBoardCurrent);

    connect(this, &TestClient::sendDubugString, &_slip, &SlipClient::on_sendDubugString);
    connect(this, &TestClient::reset, &_slip, &SlipClient::on_reset);

    connect(this, &TestClient::switchSWD, [this](){_mode = slipMode;});
    connect(this, &TestClient::switchSWD, &_slip, &SlipClient::on_switchSWD);

    connect(this, &TestClient::powerOn, [this](){_mode = slipMode;});
    connect(this, &TestClient::powerOn, &_slip, &SlipClient::on_powerOn);

    connect(this, &TestClient::powerOff, [this](){_mode = slipMode;});
    connect(this, &TestClient::powerOff, &_slip, &SlipClient::on_powerOff);

    connect(this, &TestClient::readDIN, &_slip, &SlipClient::on_readDIN);
    connect(this, &TestClient::setDOUT, &_slip, &SlipClient::on_setDOUT);
    connect(this, &TestClient::clearDOUT, &_slip, &SlipClient::on_clearDOUT);

    connect(this, &TestClient::readCSA, [this](){_mode = slipMode;});
    connect(this, &TestClient::readCSA, &_slip, &SlipClient::on_readCSA);

    connect(this, &TestClient::readAIN, &_slip, &SlipClient::on_readAIN);
    connect(this, &TestClient::configDebugSerial, &_slip, &SlipClient::on_configDebugSerial);
    connect(this, &TestClient::DaliOn, &_slip, &SlipClient::on_DaliOn);
    connect(this, &TestClient::DaliOff, &_slip, &SlipClient::on_DaliOff);
    connect(this, &TestClient::readDaliADC, &_slip, &SlipClient::on_readDaliADC);
    connect(this, &TestClient::readDinADC, &_slip, &SlipClient::on_readDinADC);
    connect(this, &TestClient::read24V, &_slip, &SlipClient::on_read24V);
    connect(this, &TestClient::read3V, &_slip, &SlipClient::on_read3V);
    connect(this, &TestClient::readTemperature, &_slip, &SlipClient::on_readTemperature);

    connect(this, &TestClient::waitCommandPrompt, &_rail, &RailtestClient::on_waitCommandPrompt);
    connect(this, &TestClient::syncCommand, &_rail, &RailtestClient::on_syncCommand);
    connect(this, &TestClient::readChipId, &_rail, &RailtestClient::on_readChipId);
    connect(this, &TestClient::testRadio, &_rail, &RailtestClient::on_testRadio);
    connect(this, &TestClient::testAccelerometer, &_rail, &RailtestClient::on_testAccelerometer);
    connect(this, &TestClient::testLightSensor, &_rail, &RailtestClient::on_testLightSensor);
    connect(this, &TestClient::testDALI, &_rail, &RailtestClient::on_testDALI);
    connect(this, &TestClient::testGNSS, &_rail, &RailtestClient::on_testGNSS);
}

TestClient::~TestClient()
{

}

void TestClient::setLogger(Logger *logger)
{
    _logger = logger;
    _rail.setLogger(logger);
    _slip.setLogger(logger);
}

void TestClient::setDutsNumbers(QList<int> numbers)
{
    _rail.setDutsNumbers(numbers);
    _slip.setDutsNumbers(numbers);
}

void TestClient::setPort(const QString &name, qint32 baudRate, QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity, QSerialPort::StopBits stopBits,
    QSerialPort::FlowControl flowControl)
{
    if (_serial.isOpen())
        _serial.close();
    _serial.setPortName(name);

    bool res = _serial.setBaudRate(baudRate)
        && _serial.setDataBits(dataBits)
        && _serial.setParity(parity)
        && _serial.setStopBits(stopBits)
        && _serial.setFlowControl(flowControl);

    if (!res)
        _logger->logError(_serial.errorString());
    open();
}

void TestClient::open()
{
    if (_serial.isOpen())
    {
        qDebug() << "Already opened " << _serial.portName();
        _serial.close();
    }

    if (!_serial.open(QSerialPort::ReadWrite))
        _logger->logError(_serial.errorString());
    else
    {
//        readCSA(0);
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

void TestClient::onSerialPortReadyRead()
{
    switch (_mode)
    {
    case idleMode:
        _serial.readAll();
        break;

    case slipMode:
        _slip.processResponsePacket();
        break;

    case railMode:
        break;
    }
}

void TestClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode)
{
    if (errorCode != QSerialPort::NoError)
        _logger->logError("Serial port error occurred " + QString().setNum(errorCode) + " on port " + _serial.portName());
}
