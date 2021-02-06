#ifndef __CONSOLEPROCESS_H__
#define __CONSOLEPROCESS_H__

#include <QProcess>
#include <QSharedPointer>
#include <QSettings>

class MainWindow;
class ConsoleProcess : public QObject
{
    Q_OBJECT

    public:
        explicit ConsoleProcess(const QSharedPointer<QSettings>& settings, QObject *parent = Q_NULLPTR);
        ~ConsoleProcess() Q_DECL_OVERRIDE;

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

    private:

        MainWindow* _mainWindow;
        QSharedPointer<QSettings> _settings;
        QProcess m_proc;
        QByteArray m_rdBuf;

    private slots:
        void readStandardOutput();
        void logError(QProcess::ProcessError error);

    signals:
        void error(QString message);
        void log(QStringList lines);
};

#endif // __CONSOLEPROCESS_H__
