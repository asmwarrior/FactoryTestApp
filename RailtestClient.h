#ifndef __RAILTESTCLIENT_H__
#define __RAILTESTCLIENT_H__

#include <QSerialPort>
#include <QVariant>

class RailtestClient : public QObject
{
    Q_OBJECT

    public:
        explicit RailtestClient(QObject *parent = Q_NULLPTR);
        virtual ~RailtestClient();

        bool open(const QString &portName);
        void close();

        bool waitCommandPrompt(int timeout = 1000);
        QVariantList syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);

        QByteArray readChipId();

    private:
        QSerialPort m_serial;
        QByteArray
            m_recvBuffer,
            m_syncCommand;
        QVariantList m_syncReplies;

        void decodeReply(const QByteArray &reply);

    private slots:
        void onSerialPortReadyRead() Q_DECL_NOTHROW;
        void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW;

    signals:
        void error(QString text);
        void replyReceived(QString id, QVariantMap params);
};

#endif // __RAILTESTCLIENT_H__
