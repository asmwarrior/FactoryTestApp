#ifndef __RAILTESTCLIENT_H__
#define __RAILTESTCLIENT_H__

#include <QDebug>

#include <QSerialPort>
#include <QVariant>
#include <QSharedPointer>
#include <QSettings>

#include "AppComponent.h"
#include "Logger.h"

class RailtestClient : public QObject, public AppComponent
{
    Q_OBJECT

public:
    explicit RailtestClient(QObject *parent = Q_NULLPTR);
    virtual ~RailtestClient();

    void setPort(const QString &portName);
    Q_INVOKABLE bool open();
    Q_INVOKABLE void close();

    Q_INVOKABLE bool waitCommandPrompt(int timeout = 1000);
    Q_INVOKABLE QVariantList syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);

    Q_INVOKABLE QByteArray readChipId();

    Q_INVOKABLE void testRadio();
    Q_INVOKABLE void testAccelerometer();
    Q_INVOKABLE void testLightSensor();
    Q_INVOKABLE void testDALI();
    Q_INVOKABLE void testGNSS();

public slots:
    void onRfReplyReceived(QString id, QVariantMap params);

private slots:
    void onSerialPortReadyRead() Q_DECL_NOTHROW;
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW;

signals:
    void error(QString text);
    void replyReceived(QString id, QVariantMap params);

private:
    QSerialPort m_serial;
    QByteArray
    m_recvBuffer,
    m_syncCommand;
    QVariantList m_syncReplies;
    int _rfRSSI;
    int _rfCount;

    void decodeReply(const QByteArray &reply);

};

#endif // __RAILTESTCLIENT_H__
