#ifndef __RAILTESTCLIENT_H__
#define __RAILTESTCLIENT_H__

#include <QDebug>

#include <QSerialPort>
#include <QVariant>
#include <QSettings>

#include "SessionManager.h"
#include "Logger.h"

class RailtestClient : public QObject
{
    Q_OBJECT

public:

    explicit RailtestClient(QSerialPort& serial, QSettings* settings, SessionManager* session, QObject *parent = Q_NULLPTR);
    virtual ~RailtestClient();

    void setLogger(Logger* logger) {_logger = logger;}
    void setDutsNumbers(QList<int> numbers);

    void setPort(const QString &portName);
    Q_INVOKABLE QByteArray currentChipId() const {return _currentChipId;}

public slots:

    void onRfReplyReceived(QString id, QVariantMap params);

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



private:

    void processResponse();
    void decodeReply(const QByteArray &reply);

    QSettings* _settings;
    SessionManager* _session;
    Logger* _logger;
    QList<int> _dutsNumbers;

    QSerialPort& m_serial;
    QByteArray m_recvBuffer;
    QByteArray m_syncCommand;
    QVariantList m_syncReplies;
    int _rfRSSI;
    int _rfCount;

    QByteArray _currentChipId;
};

#endif // __RAILTESTCLIENT_H__
