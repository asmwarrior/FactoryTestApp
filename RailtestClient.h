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
    Q_INVOKABLE QByteArray currentChipId() const {return _currentChipId;}

public slots:

    void onRfReplyReceived(QString id, QVariantMap params);

private slots:

    void onSerialPortReadyRead() Q_DECL_NOTHROW;
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError errorCode) Q_DECL_NOTHROW;

    bool on_waitCommandPrompt(int timeout = 1000);
    QVariantList on_syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void on_readChipId();
    void on_testRadio();
    void on_testAccelerometer();
    void on_testLightSensor();
    void on_testDALI();
    void on_testGNSS();

signals:

    void error(QString text);
    void replyReceived(QString id, QVariantMap params);

    void waitCommandPrompt(int timeout = 1000);
    void syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void readChipId();
    void testRadio();
    void testAccelerometer();
    void testLightSensor();
    void testDALI();
    void testGNSS();

private:

    QSerialPort m_serial;
    QByteArray
    m_recvBuffer,
    m_syncCommand;
    QVariantList m_syncReplies;
    int _rfRSSI;
    int _rfCount;

    QByteArray _currentChipId;

    void decodeReply(const QByteArray &reply);

};

#endif // __RAILTESTCLIENT_H__
