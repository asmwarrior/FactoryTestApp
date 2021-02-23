#ifndef __RAILTESTCLIENT_H__
#define __RAILTESTCLIENT_H__

#include <QDebug>

#include <QSerialPort>
#include <QVariant>
#include <QSettings>

#include "Logger.h"

class RailtestClient : public QObject
{
    Q_OBJECT

public:

    explicit RailtestClient(QSerialPort& serial, QSettings* settings, QObject *parent = Q_NULLPTR);
    virtual ~RailtestClient();

    void setLogger(Logger* logger) {_logger = logger;}

    void setPort(const QString &portName);
    Q_INVOKABLE QByteArray currentChipId() const {return _currentChipId;}

public slots:

    void onRfReplyReceived(QString id, QVariantMap params);

    void onSerialPortReadyRead() Q_DECL_NOTHROW;

    bool on_waitCommandPrompt(int timeout = 1000);
    QVariantList on_syncCommand(const QByteArray &cmd, const QByteArray &args = QByteArray(), int timeout = 5000);
    void on_readChipId();
    void on_testRadio();
    void on_testAccelerometer();
    void on_testLightSensor();
    void on_testDALI();
    void on_testGNSS();

signals:

    void commandStarted();
    void commandFinished();

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

    void decodeReply(const QByteArray &reply);

    QSettings* _settings;
    Logger* _logger;

    QSerialPort& m_serial;
    QByteArray m_recvBuffer;
    QByteArray m_syncCommand;
    QVariantList m_syncReplies;
    int _rfRSSI;
    int _rfCount;

    QByteArray _currentChipId;
};

#endif // __RAILTESTCLIENT_H__
