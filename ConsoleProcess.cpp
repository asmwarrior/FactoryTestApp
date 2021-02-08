#include "ConsoleProcess.h"

#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QCoreApplication>

ConsoleProcess::ConsoleProcess(const QSharedPointer<QSettings> &settings, QObject *parent)
    : QObject(parent), _settings(settings), m_proc(this)
{

    connect(&m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(&m_proc, &QProcess::errorOccurred, this, &ConsoleProcess::logError);
}

ConsoleProcess::~ConsoleProcess()
{
    stop();
}

void ConsoleProcess::setLogger(const QSharedPointer<Logger> &logger)
{
    _logger = logger;
}

bool ConsoleProcess::start(const QString &path, const QStringList &args, int timeout)
{
    stop();
    m_proc.setInputChannelMode(QProcess::ManagedInputChannel);
    m_proc.setProcessChannelMode(QProcess::MergedChannels);
    m_proc.start(path, args);
    if (!m_proc.waitForStarted(timeout))
    {
        emit error(tr("Cannot start external utility %1.").arg(path));

        return false;
    }

    return true;
}

bool ConsoleProcess::startJLinkScript(const QString &scriptFileName)
{
    QStringList args;

    args.append("-CommanderScript");
    args.append(_settings->value("workDirectory").toString() + scriptFileName);
    _logger->logInfo("Running JLink Commander script " + _settings->value("workDirectory").toString() + scriptFileName + "...");
    if (!start(_settings->value("JLink/path", "JLink.exe").toString(), args))
    {
        _logger->logError("Cannot start JLink Commander!");
        return false;
    }
        //throw InputOutputError("Cannot start JLink Commander!");

    if (!skipUntilFinished())
    {
        return false;
    }
        //throw InputOutputError("JLink Commander was executed too long!");

    if (exitCode())
    {
        _logger->logInfo("JLink Commander script failed!.");
        return false;
    }
        //throw TestError("Error while executing JLink Commander script!");

    _logger->logInfo("JLink Commander script completed.");

    return true;
}

void ConsoleProcess::stop()
{
    if (m_proc.state() != QProcess::NotRunning)
    {
        m_proc.kill();
        m_rdBuf.clear();
    }
}

bool ConsoleProcess::isRunning()
{
    return m_proc.state() == QProcess::Running;
}

int ConsoleProcess::exitCode()
{
    return m_proc.exitStatus() == QProcess::NormalExit ? m_proc.exitCode() : 0x7FFFFFFFL;
}

void ConsoleProcess::readStandardOutput()
{
    QByteArray data = m_proc.readAllStandardOutput();

    data.replace('\0', ' ');
    //if (receivers(SIGNAL(log(QStringList))) > 0)
    {
        QStringList lines = QString::fromLocal8Bit(data).split("\r\n");
        emit log(lines);
        for(auto & line : lines)
        {
            if(!line.isEmpty())
                _logger->logInfo(line);
        }
    }
    m_rdBuf.append(data);
}

void ConsoleProcess::logError(QProcess::ProcessError error)
{
    qDebug() << error;
}

bool ConsoleProcess::write(const QByteArray &data)
{
    if (m_proc.state() != QProcess::Running)
    {
        emit error(tr("External utility is not started."));

        return false;
    }

    if (m_proc.write(data) != data.size())
    {
        emit error(tr("Write error to the external utility stdin."));

        return false;
    }

    return true;
}

QByteArray ConsoleProcess::read()
{
    QByteArray ret = m_rdBuf;

    m_rdBuf.clear();

    return ret;
}

bool ConsoleProcess::readUntilExpected(const QByteArray &expected, QByteArray &received, int timeout)
{
    received.clear();

    do {
        QCoreApplication::processEvents();

        int index = m_rdBuf.indexOf(expected);

        if (index < 0)
        {
            received.append(m_rdBuf);
            m_rdBuf.clear();
            QThread::msleep(500);
            timeout -= 500;
        }
        else
        {
            index += expected.length();
            received.append(m_rdBuf.left(index));
            m_rdBuf.remove(0, index);

            return true;
        }
    } while(timeout > 0);

    return false;
}

bool ConsoleProcess::skipUntilExpected(const QByteArray &expected, int timeout)
{
    do {
        QCoreApplication::processEvents();

        int index = m_rdBuf.indexOf(expected);

        if (index < 0)
        {
            m_rdBuf.clear();
            QThread::msleep(500);
            timeout -= 500;
        }
        else
        {
            m_rdBuf.remove(0, index + expected.length());

            return true;
        }
    } while(timeout > 0);

    return false;
}

void ConsoleProcess::clear()
{
    m_rdBuf.clear();
}

bool ConsoleProcess::readUntilFinished(QByteArray &received, int timeout)
{
    if (m_proc.state() != QProcess::Running)
        return false;

    int ret = m_proc.waitForFinished(timeout);

    received = m_rdBuf;
    m_rdBuf.clear();
    received.append(m_proc.readAllStandardOutput());

    return ret;
}

bool ConsoleProcess::skipUntilFinished(int timeout)
{
    if (m_proc.state() != QProcess::Running)
        return true;

    int ret = m_proc.waitForFinished(timeout);

    m_rdBuf.clear();

    return ret;
}
