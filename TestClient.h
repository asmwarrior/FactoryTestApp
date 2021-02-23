#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QObject>
#include <QQueue>
#include <QTimer>

#include "RailtestClient.h"
#include "SlipClient.h"

class TestClient : public QObject
{
    Q_OBJECT

public:

    enum Mode {idleMode, railMode, slipMode};

    struct Command
    {
        QString name;
        QVariantList args;
    };

    explicit TestClient(QSettings* settings, QObject *parent = nullptr);
    ~TestClient();

    void setLogger(Logger* logger);

    void setPort(const QString &name,
                 qint32 baudRate = QSerialPort::Baud115200,
                 QSerialPort::DataBits dataBits = QSerialPort::Data8,
                 QSerialPort::Parity parity = QSerialPort::NoParity,
                 QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                 QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    void open();
    void close();

    void setMode(Mode mode) {_mode = mode;}

private slots:

    void onSerialPortReadyRead();
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode);
    void processCommandQueue();
    void on_waitCommandsExecTimerTimeout();

public slots:

    void addCommand(const QString& name, const QVariantList& args = {}) {_commands.enqueue({name, args});}

signals:

private:

    QSettings* _settings;
    Logger* _logger;

    Mode _mode = idleMode;
    QSerialPort _serial;
    RailtestClient _rail;
    SlipClient _slip;

    QQueue<Command> _commands;
    QTimer* _processCommandsTimer;
    QTimer* _waitCommandsExecTimer;
};

#endif // TESTCLIENT_H
