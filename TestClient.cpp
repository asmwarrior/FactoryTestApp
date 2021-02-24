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
    connect(&_serial, &QSerialPort::errorOccurred, this, &TestClient::onSerialPortErrorOccurred);

    connect(this, &TestClient::switchSWD, &_slip, &SlipClient::on_switchSWD);
    connect(this, &TestClient::readCSA, &_slip, &SlipClient::on_readCSA);
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

void TestClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode)
{
    if (errorCode != QSerialPort::NoError)
        _logger->logError("Serial port error occurred.");
}
