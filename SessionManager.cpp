#include "SessionManager.h"

SessionManager::SessionManager(QSettings *settings, QObject *parent) : QObject(parent), _settings(settings)
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

    QFile file(_settings->value("workDirectory").toString() + "/log.txt");

    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        file.write(record.id.toLocal8Bit() + " " + record.batchNumber.toLocal8Bit() + " " + record.operatorName.toLocal8Bit() + " " + record.timeStamp.toLocal8Bit() + " " + record.state.toLocal8Bit() + "\n");
    }

    file.close();

    _records.clear();
}
