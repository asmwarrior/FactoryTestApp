#include "ConsoleProcess.h"

#include <QProcess>
#include <QThread>
#include <QCoreApplication>

ConsoleProcess::ConsoleProcess(QObject *parent)
    : QObject(parent), m_proc(this)
{
    connect(&m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
}

ConsoleProcess::~ConsoleProcess()
{
    stop();
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
    if (receivers(SIGNAL(log(QStringList))) > 0)
    {
        QStringList lines = QString::fromLocal8Bit(data).split("\r\n");

        emit log(lines);
    }
    m_rdBuf.append(data);
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
