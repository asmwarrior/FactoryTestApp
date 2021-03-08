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
    record.no = dut["no"].toString();
    record.error = dut["error"].toString();
    record.batchNumber = _batchNumber;
    record.method = _method;
    record.operatorName = _operatorName;
    record.timeStamp = _startTime;

    if(dut["state"].toInt() != DutState::tested)
    {
        _failedCount++;
        record.state = "FAILED";
    }
    else
    {
        _successCount++;
        record.state = "PASSED";
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
    _method = "";
    _successCount = 0;
    _failedCount = 0;

    emit sessionStatsChanged();
}

void SessionManager::writeDutRecordsToDatabase()
{
    for(auto & record : _records)
    {
        _db->insertIntoTable(record);

        if(record.state == "PASSED")
        {
            emit printLabel(record);
        }
    }

    QFile file(_settings->value("workDirectory").toString() + "/log.txt");

    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        file.write(    "ID: " +  record.id.toLocal8Bit() + "; "
                     + "SOCKET: " +  record.no.toLocal8Bit() + "; "
                     + "BATCH NUMBER: " +  record.batchNumber.toLocal8Bit() + "; "
                     + "TEST METHOD: " +  record.method.toLocal8Bit() + "; "
                     + "OPERATOR: " +  record.operatorName.toLocal8Bit() + "; "
                     + "DATE: " +  record.timeStamp.toLocal8Bit() + "; "
                     + "RESULT: " +  record.state.toLocal8Bit());

        if(record.error.size())
        {
            file.write(" ERROR DESC: " + record.error.toLocal8Bit() + "\n");
        }

        else
        {
            file.write("\n");
        }
    }

    file.close();

    _records.clear();
}
