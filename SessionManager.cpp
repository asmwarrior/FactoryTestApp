#include "SessionManager.h"

SessionManager::SessionManager(QSettings *settings, QObject *parent) : QObject(parent)
{
    //Database
    _db = new DataBase(settings, this);
    _db->connectToDataBase();
//    _db->createTable();
}

SessionManager::~SessionManager()
{

}

void SessionManager::logDutInfo(Dut dut)
{
    DutRecord record;
    record.id = dut["id"].toString();
    record.batchNumber = _batchNumber;
    record.operatorName = _operatorName;
    record.timeStamp = _startTime;

    if(dut["state"].toInt() != DutState::tested)
    {
        _failedCount++;
        record.state = "testing failed";
    }
    else
    {
        _successCount++;
        record.state = "testing successfully";
    }

    _records.push_back(record);


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

void SessionManager::writeDutRecordsToDatabase()
{
    for(auto & record : _records)
    {
        _db->insertIntoTable(record);
    }

    _records.clear();
}
