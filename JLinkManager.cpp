#include "JLinkManager.h"

#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QCoreApplication>

JLinkManager::JLinkManager(const QSharedPointer<QSettings> &settings, QObject *parent)
    : QObject(parent), _settings(settings), _proc(this)
{
    _proc.setProgram(_settings->value("JLink/path").toString());
    QObject::connect(&_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    QObject::connect(this, &JLinkManager::startScript, this, &JLinkManager::on_startScript);
    QObject::connect(this, &JLinkManager::establishConnection, this, &JLinkManager::on_establishConnection);
    clearErrorBuffer();
}

JLinkManager::~JLinkManager()
{

}

void JLinkManager::clearErrorBuffer()
{
    _errorBuffer = {256, '\0'};
}

void JLinkManager::setSN(const QString &serialNumber)
{
    _SN = serialNumber;
}

QString JLinkManager::getSN() const
{
    return _SN;
}

bool JLinkManager::isConnected() const
{
    if(_state == State::connectionTested)
        return true;

    return false;
}

void JLinkManager::selectByUSB()
{
    if (JLINKARM_EMU_SelectByUSBSN(_SN.toUInt()) < 0)
    {
        _state = unknown;
        _logger->logError("No connection to JLink with S/N " + _SN);
    }
}

void JLinkManager::open()
{
    if(JLINKARM_Open())
        _logger->logError("JLINK: An error occured when opening JLink programmer.");
}

void JLinkManager::setDevice(const QString &device)
{
    clearErrorBuffer();
    char cmd[0x400];
    strcpy_s(cmd, "device = ");
    strcat_s(cmd, device.toLocal8Bit().data());
    JLINKARM_ExecCommand(cmd, _errorBuffer.data(), _errorBuffer.size());
    if(_errorBuffer.at(0) != 0)
    {
        _logger->logError("JLINK: " + _errorBuffer);
    }
}

void JLinkManager::select(int interface)
{
    _targetInterface = interface;
    JLINKARM_TIF_Select(_targetInterface);
}

void JLinkManager::setSpeed(int speed)
{
    _speed = speed;
    JLINKARM_SetSpeed(_speed);
}

void JLinkManager::connect()
{
    if(JLINKARM_Connect())
    {
        _logger->logError("JLINK: Could not connect to target.");
    }
}

int JLinkManager::erase()
{
    int error = 0;
    error = JLINK_EraseChip();
    return error;
}

void JLinkManager::reset()
{
    JLINKARM_Reset();
}

void JLinkManager::go()
{
    JLINKARM_Go();
}

int JLinkManager::downloadFile(const QString &fileName, int adress)
{
    int error = 0;
    JLINKARM_BeginDownload(0); // Indicates start of flash download
    error = JLINK_DownloadFile(QString(_settings->value("workDirectory").toString() + "/" + fileName).toLocal8Bit().data(), adress); // Load the application binary to address 0
    JLINKARM_EndDownload();
    return error;
}

void JLinkManager::close()
{
    JLINKARM_Close();
}

void JLinkManager::on_establishConnection()
{
    if(_SN.isEmpty())
    {
        _logger->logError("No serial number for the JLink device provided.");
        return;
    }

    _state = waitingTestResponse;

    if (JLINKARM_EMU_SelectByUSBSN(_SN.toUInt()) < 0)
    {
        _state = unknown;
        _logger->logError("No connection to JLink with S/N " + _SN);
    }

    else
    {
        _state = connectionTested;
        _logger->logSuccess("JLink with S/N: " + _SN + " connected");
    }
}

void JLinkManager::on_startScript(const QString &scriptFile)
{
    _proc.setArguments({"-USB", _SN, "-CommanderScript", QString(_settings->value("workDirectory").toString() + "/" + scriptFile)});
    _proc.start();
    _proc.waitForStarted(2000);
}

void JLinkManager::readStandardOutput()
{
    QByteArray data = _proc.readAllStandardOutput();

    data.replace('\0', ' ');
    QStringList lines = QString::fromLocal8Bit(data).split("\r\n");
    for(auto & line : lines)
    {
        if(line.size())
            qDebug() << line;
    }
}

