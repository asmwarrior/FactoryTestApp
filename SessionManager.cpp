#include "SessionManager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent)
{

}

SessionManager::~SessionManager()
{

}

void SessionManager::logDutInfo(Dut dut)
{
    if(dut["state"].toInt() != DutState::tested)
        _failedCount++;
    else
        _successCount++;

    emit sessionStatsChanged();
}

void SessionManager::clear()
{
    _operatorName = "";
    _startTime = "";
    _batchNumber = "";
    _batchInfo = "";
    _successCount = 0;
    _failedCount = 0;

    emit sessionStatsChanged();
}
