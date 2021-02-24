#include "JLinkManager.h"

#include "JLinkSDK/JLinkARMDLL.h"

#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QCoreApplication>

JLinkManager::JLinkManager(QSettings *settings, QObject *parent)
    : QObject(parent), _settings(settings)
{
    connect(this, &JLinkManager::startJlinkCommands, this, &JLinkManager::on_startJlinkCommands);
    connect(this, &JLinkManager::establishConnection, this, &JLinkManager::on_establishConnection);
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

void JLinkManager::on_establishConnection()
{
    if(_SN.isEmpty())
    {
        _logger->logError("No serial number for the JLink device provided!");
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

void JLinkManager::on_startJlinkCommands(const QStringList &commands)
{
    for(auto & command : commands)
    {
        clearErrorBuffer();
        JLINKARM_ExecCommand(command.toLatin1().data(), _errorBuffer.data(), _errorBuffer.size());
        if(_errorBuffer.at(0) != 0)
        {
            _logger->logError("JLINK: " + _errorBuffer);
            break;
        }
    }
}

