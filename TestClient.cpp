#include "TestClient.h"

#include <QTimer>

TestClient::TestClient(QSettings *settings, QObject *parent)
    : QObject(parent),
      _settings(settings),
      _serial(this),
      _rail(_serial, settings, this),
      _slip(_serial, this)
{
    connect(&_serial, &QSerialPort::readyRead, this, &TestClient::onSerialPortReadyRead);
    connect(&_serial, &QSerialPort::errorOccurred, this, &TestClient::onSerialPortErrorOccurred);

    connect(&_rail, &RailtestClient::commandStarted, [this](){setMode(railMode);});
    connect(&_rail, &RailtestClient::commandFinished, [this](){setMode(idleMode); _waitCommandsExecTimer->stop();});
    connect(&_slip, &SlipClient::commandStarted, [this](){setMode(slipMode);});
    connect(&_slip, &SlipClient::commandFinished, [this](){setMode(idleMode);; _waitCommandsExecTimer->stop();});

    _processCommandsTimer = new QTimer(this);
    connect(_processCommandsTimer, &QTimer::timeout, this, &TestClient::processCommandQueue);
    _processCommandsTimer->start(200);

    _waitCommandsExecTimer = new QTimer(this);
    connect(_waitCommandsExecTimer, &QTimer::timeout, this, &TestClient::on_waitCommandsExecTimerTimeout);
}

TestClient::~TestClient()
{
    _processCommandsTimer->stop();
    _waitCommandsExecTimer->stop();
}

void TestClient::setLogger(Logger *logger)
{
    _logger = logger;
    _rail.setLogger(logger);
    _slip.setLogger(logger);
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
        _serial.close();

    if (!_serial.open(QSerialPort::ReadWrite))
        _logger->logError(_serial.errorString());
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
    case railMode:
        _rail.onSerialPortReadyRead();
        break;

    case slipMode:
        _slip.onSerialPortReadyRead();
        break;

    case idleMode:
        break;
    }

}

void TestClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode)
{
    if (errorCode != QSerialPort::NoError)
        _logger->logError("Serial port error occurred.");
}

void TestClient::processCommandQueue()
{
    if(_commands.isEmpty() || _mode != Mode::idleMode)
        return;

    auto command = _commands.dequeue();

    if(command.name == "switch SWD")
    {
        setMode(slipMode);
        _slip.switchSWD(command.args);
    }

    else if(command.name == "read chip ID")
    {
        setMode(railMode);
        _rail.readChipId();
    }

    _waitCommandsExecTimer->start(3000);
}

void TestClient::on_waitCommandsExecTimerTimeout()
{
    setMode(idleMode);
    _waitCommandsExecTimer->stop();
    _logger->logError("No response to the test command was recieved!");
}
