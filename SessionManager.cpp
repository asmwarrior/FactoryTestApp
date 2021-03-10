#include "SessionManager.h"

SessionManager::SessionManager(QSettings *settings, QObject *parent) : QObject(parent), _settings(settings)
{
    //Database
    _db = new DataBase(settings, this);
    _db->connectToDataBase();
//    _db->createTable();

    auto sep = _settings->value("Report/csv_separator").toByteArray();
    if(sep == "")
        _csv_separator = ";";
    else
        _csv_separator = sep;

    _runningNumber = _settings->value("Report/runningNumber").toInt();
}

SessionManager::~SessionManager()
{

}

void SessionManager::logDutInfo(Dut dut)
{
    _runningNumber++;
    _settings->setValue("Report/runningNumber", _runningNumber);
    DutRecord record;

    QString zeroFields;
    if(_runningNumber < 10)
        zeroFields = "000";
    else if(_runningNumber < 100)
        zeroFields = "00";
    else if(_runningNumber < 1000)
        zeroFields = "0";

    record.runningNumber = zeroFields + QString().setNum(_runningNumber);

    record.id = dut["id"].toString();
    record.no = dut["no"].toString();
    record.error = dut["error"].toString().remove('\n').remove('\r').remove(';').remove(',');
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
    _testCyclesCount = 0;

    emit sessionStatsChanged();
}

void SessionManager::writeDutRecordsToDatabase()
{
    for(auto & record : _records)
    {
        record.cycleNo = QString().setNum(_testCyclesCount);
        _db->insertIntoTable(record);

        if(record.state == "PASSED")
        {
            emit printLabel(record);
        }
    }

    //Text log file

    QFile file(_settings->value("workDirectory").toString() + "/reports/log.txt");

    file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        file.write(    record.runningNumber.toLocal8Bit() + "; "
                     + "ID: " +  record.id.toLocal8Bit() + "; "
                     + "SOCKET: " +  record.no.toLocal8Bit() + "; "
                     + "TEST CYCLE: " +  record.cycleNo.toLocal8Bit() + "; "
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

    //CSV log file

    QFile csv_file(_settings->value("workDirectory").toString() + "/reports/" + "ALL_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".csv");

    csv_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        csv_file.write(    record.runningNumber.toLocal8Bit() + _csv_separator
                     + record.id.toLocal8Bit() + _csv_separator
                     + record.no.toLocal8Bit() + _csv_separator
                     + record.cycleNo.toLocal8Bit() + _csv_separator
                     + record.batchNumber.toLocal8Bit() + _csv_separator
                     + record.method.toLocal8Bit() + _csv_separator
                     + record.operatorName.toLocal8Bit() + _csv_separator
                     + record.timeStamp.toLocal8Bit() + _csv_separator
                     + record.state.toLocal8Bit());

        if(record.error.size())
        {
            csv_file.write(_csv_separator + record.error.toLocal8Bit());
        }
        csv_file.write(_csv_separator + "\n");
    }


    csv_file.close();

    //Brief CSV log file

    QFile brief_csv_file(_settings->value("workDirectory").toString() + "/reports/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".csv");

    brief_csv_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        brief_csv_file.write(    record.runningNumber.toLocal8Bit() + _csv_separator
                                 + record.cycleNo.toLocal8Bit() + _csv_separator
                                 + record.no.toLocal8Bit() + _csv_separator
                                 + record.id.toLocal8Bit());

        if(record.state == "FAILED")
        {
            brief_csv_file.write(_csv_separator + "FALIED");
        }
        brief_csv_file.write(_csv_separator + "\n");
    }

    brief_csv_file.close();

    //CSV printer log file

    QFile printer_csv_file(_settings->value("workDirectory").toString() + "/reports/" + "PRINTER_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + " " + _method + ".csv");

    printer_csv_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    for(auto & record : _records)
    {
        printer_csv_file.write(    record.runningNumber.toLocal8Bit() + _csv_separator
                                   + record.cycleNo.toLocal8Bit() + "."
                                   + record.no.toLocal8Bit() + _csv_separator
                                   + record.id.toLocal8Bit() + _csv_separator
                                   + record.batchNumber.toLocal8Bit());

        if(record.state == "FAILED")
        {
            printer_csv_file.write(_csv_separator + "FAILED");
        }
        printer_csv_file.write(_csv_separator + "\n");
    }


    printer_csv_file.close();

    _records.clear();
}
