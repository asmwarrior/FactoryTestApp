#include "SlipClient.h"

#include <QCoreApplication>

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

SlipClient::SlipClient(QObject *parent)
    : QObject(), m_thread(parent), m_serialPort(this), m_frameCnt(0)
{
    const QMetaObject* const mo = metaObject();

    m_sendMethod = mo->method(mo->indexOfSlot("sendFrame(int,QByteArray)"));

    connect(&m_thread, &QThread::started, this, &SlipClient::opened);
    connect(&m_thread, &QThread::finished, this, &SlipClient::onThreadFinished);

    connect(&m_serialPort, &QSerialPort::readyRead, this, &SlipClient::onSerialPortReadyRead);
    connect(&m_serialPort, &QSerialPort::errorOccurred, this, &SlipClient::onSerialPortErrorOccurred);
    connect(&m_serialPort, &QSerialPort::aboutToClose, this, &SlipClient::aboutToClose);
    connect(this, &SlipClient::packetReceived, this, &SlipClient::onSlipPacketReceived);
}

SlipClient::~SlipClient()
{
    close();
}

void SlipClient::setPort(const QString &name, qint32 baudRate, QSerialPort::DataBits dataBits,
    QSerialPort::Parity parity, QSerialPort::StopBits stopBits,
    QSerialPort::FlowControl flowControl)
{
    if (m_thread.isRunning())
        /*throw SerialPortError(QSerialPort::PermissionError, "SLIP serial port already opened")*/;

    if (thread() != QThread::currentThread())
        /*throw SerialPortError(QSerialPort::UnsupportedOperationError, "cannot change SLIP serial port parameters from different thread")*/;

    if (m_serialPort.isOpen())
        m_serialPort.close();
    m_serialPort.setPortName(name);

    bool res = m_serialPort.setBaudRate(baudRate)
        && m_serialPort.setDataBits(dataBits)
        && m_serialPort.setParity(parity)
        && m_serialPort.setStopBits(stopBits)
        && m_serialPort.setFlowControl(flowControl);

    if (!res)
        /*throw SerialPortError(m_serialPort.error(), m_serialPort.errorString())*/;
}

void SlipClient::open()
{
    if (m_thread.isRunning())
        /*throw SerialPortError(QSerialPort::PermissionError, "SLIP serial port already opened")*/;

    if (thread() != QThread::currentThread())
        /*throw SerialPortError(QSerialPort::UnsupportedOperationError, "cannot open SLIP serial port from different thread")*/;

    if (m_serialPort.isOpen())
        m_serialPort.close();
    cleanup();
    if (!m_serialPort.open(QSerialPort::ReadWrite))
        /*throw SerialPortError(m_serialPort.error(), m_serialPort.errorString())*/;

    m_origin = thread();
    moveToThread(&m_thread);
    m_thread.start(QThread::HighestPriority);
}

void SlipClient::close() Q_DECL_NOTHROW
{
    m_thread.quit();
    m_thread.wait();
}

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

quint8 SlipClient::nextFrameId() Q_DECL_NOTHROW
{
    QMutexLocker lock(&m_attrLock);

    ++m_frameCnt;
    if (!m_frameCnt)
        m_frameCnt = 1;

    return m_frameCnt;
}

void SlipClient::sendPacket(quint8 channel, const QByteArray &frame) Q_DECL_NOTHROW
{
    m_sendMethod.invoke(this, Qt::QueuedConnection, Q_ARG(quint8, channel), Q_ARG(QByteArray, frame));
}

void SlipClient::sendFrame(int channel, const QByteArray &frame) Q_DECL_NOTHROW
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
    m_serialPort.write(encodedBuffer);
}

void SlipClient::cleanup() Q_DECL_NOTHROW
{
    m_frameStarted = false;
    m_recvBuffer.clear();
}

void SlipClient::decodeFrame() Q_DECL_NOTHROW
{
    QByteArray decodedBuffer;

    // Write unescaped buffer.
    decodedBuffer.reserve(m_recvBuffer.size());
    for (int i = 0; i < m_recvBuffer.size(); ++i)
    {
        char ch = m_recvBuffer.at(i);

        if (ch == ESC_SLIP_OCTET)
        {
            ++i;
            if (i >= m_recvBuffer.size())
            {
                /*emit error(new SlipError("unfinished escape sequence", m_recvBuffer))*/;

                return;
            }
            switch (m_recvBuffer.at(i))
            {
                case END_SUBS_OCTET:
                    decodedBuffer.append(END_SLIP_OCTET);
                    break;

                case ESC_SUBS_OCTET:
                    decodedBuffer.append(ESC_SLIP_OCTET);
                    break;

                default:
                    /*emit error(new SlipError("invalid escape sequence", m_recvBuffer))*/;

                    return;
            }
        }
        else
            decodedBuffer.append(ch);
    }
    if (decodedBuffer.size() < MIN_FRAME_SIZE)
    {
        /*emit error(new SlipError("Frame too short", decodedBuffer))*/;

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
        /*emit error(new SlipError("invalid Frame CRC", decodedBuffer))*/;

        return;
    }

    // Frame received successfully.
    emit packetReceived(decodedBuffer.at(0), decodedBuffer.mid(1, frameSize - 1));
}

void SlipClient::onSerialPortReadyRead() Q_DECL_NOTHROW
{
    while (m_serialPort.bytesAvailable())
    {
        QByteArray buffer = m_serialPort.readAll();

        foreach (char ch, buffer)
            if (ch == END_SLIP_OCTET)
                if (m_frameStarted)
                {
                    if (m_recvBuffer.size() >= MIN_FRAME_SIZE)
                    {
                        decodeFrame();
                        m_frameStarted = false;
                    }
                    m_recvBuffer.clear();
                }
                else
                {
                   m_frameStarted = true;
                   m_recvBuffer.clear();
                }
            else
                if (m_frameStarted)
                    m_recvBuffer.append(ch);
    }
}

void SlipClient::onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW
{
    if (errorCode != QSerialPort::NoError)
        /*emit error(new SerialPortError(errorCode, m_serialPort.errorString()))*/;
}

void SlipClient::onThreadFinished() Q_DECL_NOTHROW
{
    if (m_serialPort.isOpen())
    {
        m_serialPort.flush();
        m_serialPort.close();
    }
    cleanup();
    moveToThread(m_origin);
    if (thread() != m_origin)
        moveToThread(QCoreApplication::instance()->thread());
}

void SlipClient::onSlipPacketReceived(quint8 channel, QByteArray frame) noexcept
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
                        logger->logInfo("Startup event.");
                        break;

                    case MB_GENERAL_RESULT:
                        {
                            MB_GeneralResult_t *gr = (MB_GeneralResult_t*)pkt;

                            gr->errorCode = qFromBigEndian(gr->errorCode);
                            logger->logInfo(QString("RESULT: cmd=%1, code=%2.").arg(gr->header.sequence).arg(gr->errorCode));
                        }
                        break;

                    case MB_ASYNC_EVENT:
                        {
                            MB_Event_t *evt = (MB_Event_t*)pkt;

                            evt->eventCode = qFromBigEndian(evt->eventCode);
                            logger->logInfo(QString("EVENT: code=%2.").arg(evt->eventCode));
                        }
                        break;
                }
            }
            break;

        case 1:
            logger->logInfo(frame);
            break;

        case 2:
            logger->logInfo(frame);
            break;

        case 3:
            logger->logInfo(frame);
            break;
    }
}
