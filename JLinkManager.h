#pragma once

#include <QProcess>
#include <QSharedPointer>
#include <QSettings>

#include "Logger.h"

class JLinkManager : public QObject
{
    Q_OBJECT

    public:
        explicit JLinkManager(const QSharedPointer<QSettings> &settings, QObject *parent = Q_NULLPTR);
        ~JLinkManager() Q_DECL_OVERRIDE;

        void setLogger(const QSharedPointer<Logger>& logger);

        Q_INVOKABLE bool start(const QString &path, const QStringList &args = QStringList(), int timeout = 30000);
        Q_INVOKABLE bool startJLinkScript(const QString& scriptFileName);
        void stop();
        bool isRunning();
        int exitCode();

        bool write(const QByteArray &data);
        QByteArray read();
        bool readUntilExpected(const QByteArray &expected, QByteArray &received, int timeout = 30000);
        bool skipUntilExpected(const QByteArray &expected, int timeout = 30000);
        void clear();
        bool readUntilFinished(QByteArray &received, int timeout = 30000);
        bool skipUntilFinished(int timeout = 30000);

private slots:
    void readStandardOutput();
    void logError(QProcess::ProcessError error);

signals:
    void error(QString message);
    void log(QStringList lines);

private:

        QSharedPointer<QSettings> _settings;
        QSharedPointer<Logger> _logger;
        QProcess m_proc;
        QByteArray m_rdBuf;
};
