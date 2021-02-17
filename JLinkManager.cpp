#include "JLinkManager.h"

#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QCoreApplication>

JLinkManager::JLinkManager(QObject *parent)
    : QObject(parent)/*, m_proc(this)*/
{
    connect(&m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(&m_proc, &QProcess::errorOccurred, this, &JLinkManager::logError);
    connect(this, SIGNAL(startScript(const QString&)), this, SLOT(startJLinkScript(const QString&)), Qt::ConnectionType::QueuedConnection);
}

JLinkManager::~JLinkManager()
{
    stop();
}

void JLinkManager::setSN(const QString &serialNumber)
{
    _SN = serialNumber;
}

QString JLinkManager::getSN() const
{
    return _SN;
}

bool JLinkManager::start(const QString &path, const QStringList &args, int timeout)
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
    m_proc.write("\r");
    return true;
}

bool JLinkManager::startJLinkScript(const QString &scriptFileName)
{
    QStringList args;

    args.append("-USB");
    args.append(_SN);
//    args.append("-CommanderScript");
    args.append("-CommandFile");
    args.append(settings->value("workDirectory").toString() + scriptFileName);
    logger->logInfo("Running JLink Commander script " + settings->value("workDirectory").toString() + scriptFileName + "...");
    if (!start(settings->value("JLink/path", "JLink.exe").toString(), args))
    {
        logger->logError("Cannot start JLink Commander!");
        return false;
    }

    if (!skipUntilFinished())
    {
        stop();
        return false;
    }

    if (exitCode())
    {
        logger->logInfo("JLink Commander script failed!.");
        return false;
    }

    logger->logInfo("JLink Commander script completed.");

    return true;
}

void JLinkManager::stop()
{
    if (m_proc.state() != QProcess::NotRunning)
    {
        m_proc.kill();
        m_rdBuf.clear();
    }
}

bool JLinkManager::isRunning()
{
    return m_proc.state() == QProcess::Running;
}

int JLinkManager::exitCode()
{
    return m_proc.exitStatus() == QProcess::NormalExit ? m_proc.exitCode() : 0x7FFFFFFFL;
}

void JLinkManager::readStandardOutput()
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
                logger->logInfo(line);
        }
    }
    m_rdBuf.append(data);
}

void JLinkManager::logError(QProcess::ProcessError error)
{
    qDebug() << error;
}

bool JLinkManager::write(const QByteArray &data)
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

QByteArray JLinkManager::read()
{
    QByteArray ret = m_rdBuf;

    m_rdBuf.clear();

    return ret;
}

bool JLinkManager::readUntilExpected(const QByteArray &expected, QByteArray &received, int timeout)
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

bool JLinkManager::skipUntilExpected(const QByteArray &expected, int timeout)
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

void JLinkManager::clear()
{
    m_rdBuf.clear();
}

bool JLinkManager::readUntilFinished(QByteArray &received, int timeout)
{
    if (m_proc.state() != QProcess::Running)
        return false;

    int ret = m_proc.waitForFinished(timeout);

    received = m_rdBuf;
    m_rdBuf.clear();
    received.append(m_proc.readAllStandardOutput());

    return ret;
}

bool JLinkManager::skipUntilFinished(int timeout)
{
    if (m_proc.state() != QProcess::Running)
        return true;

    int ret = m_proc.waitForFinished(timeout);

    m_rdBuf.clear();

    return ret;
}
