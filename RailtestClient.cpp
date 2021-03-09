#include "RailtestClient.h"

#include <QCoreApplication>
#include <QTime>

RailtestClient::RailtestClient(QObject *parent)
    : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &RailtestClient::onSerialPortReadyRead);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &RailtestClient::onSerialPortErrorOccurred);
}

RailtestClient::~RailtestClient()
{
    close();
}

bool RailtestClient::open(const QString &portName)
{
    close();
    m_serial.setPortName(portName);
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    return m_serial.open(QSerialPort::ReadWrite);
}

void RailtestClient::close()
{
    if (m_serial.isOpen())
    {
        m_serial.flush();
        m_serial.close();
        m_recvBuffer.clear();
    }
}

bool RailtestClient::waitCommandPrompt(int timeout)
{
    if (!m_serial.isOpen())
        return false;

    m_syncCommand = "__waitCommandPrompt__";

    QTime expire = QTime::currentTime().addMSecs(timeout);

    m_serial.write("\r\n");
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
        if (m_syncCommand.isEmpty())
            return true;
    }

    return false;
}

QVariantList RailtestClient::syncCommand(const QByteArray &cmd, const QByteArray &args, int timeout)
{
    if (!m_serial.isOpen())
        return QVariantList();

    m_syncCommand = cmd;
    m_syncReplies.clear();

    QTime expire = QTime::currentTime().addMSecs(timeout);

    m_serial.write(cmd + " " + args + "\r\n");
    while (QTime::currentTime() <= expire)
    {
        QCoreApplication::processEvents();
        if (m_syncCommand.isEmpty())
            return m_syncReplies;
    }

    QVariantMap error;

    error.insert("error", "timeout");
    error.insert("errorCode", 1);
    m_syncReplies.append(error);

    return m_syncReplies;
}

void RailtestClient::decodeReply(const QByteArray &reply)
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

        if (m_syncCommand == name)
            m_syncReplies.append(decodedParams);
        else
            emit replyReceived(name, decodedParams);

        return;
    }

    if (reply.startsWith("{{") && reply.endsWith("}}"))
    {
        auto params = reply.mid(1, reply.length() - 2).split('}');
        QVariantList decodedParams;

        foreach (auto param, params)
            if (param.startsWith('{'))
                decodedParams.append(param.mid(1));

        if (!m_syncCommand.isEmpty())
            m_syncReplies.push_back(decodedParams);

        return;
    }
}

void RailtestClient::onSerialPortReadyRead() Q_DECL_NOTHROW
{
    while (m_serial.bytesAvailable())
        m_recvBuffer += m_serial.readAll();

    int idx = m_recvBuffer.indexOf("\r\n");

    while (idx != -1)
    {
        auto reply = m_recvBuffer.left(idx);

        decodeReply(reply);

        m_recvBuffer = m_recvBuffer.mid(idx + 2);
        idx = m_recvBuffer.indexOf("\r\n");
    }

    if (m_recvBuffer == "> ")
        m_syncCommand.clear();
}

void RailtestClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW
{
    if (errorCode != QSerialPort::NoError)
        emit error(m_serial.errorString());
}
