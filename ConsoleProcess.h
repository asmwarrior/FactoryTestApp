#ifndef __CONSOLEPROCESS_H__
#define __CONSOLEPROCESS_H__

#include <QProcess>

class ConsoleProcess : public QObject
{
    Q_OBJECT

    public:
        explicit ConsoleProcess(QObject *parent = Q_NULLPTR);
        ~ConsoleProcess() Q_DECL_OVERRIDE;

        bool start(const QString &path, const QStringList &args = QStringList(), int timeout = 30000);
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

    private:
        QProcess m_proc;
        QByteArray m_rdBuf;

    private slots:
        void readStandardOutput();

    signals:
        void error(QString message);
        void log(QStringList lines);
};

#endif // __CONSOLEPROCESS_H__
