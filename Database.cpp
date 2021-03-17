#include "Database.h"

DataBase::DataBase(const QSharedPointer<QSettings> &settings, QObject *parent) : QObject(parent), _settings(settings)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{
    this->openDataBase();
}

bool DataBase::restoreDataBase()
{
//    if(this->openDataBase())
//    {
//        return (this->createTable()) ? true : false;
//    }
//    else
//    {
//        qDebug() << "Couldn't open database!";
//        return false;
//    }
    return false;
}

bool DataBase::openDataBase()
{
    _db = QSqlDatabase::addDatabase("QPSQL");
    //_db.setHostName("localhost");
    //_db.setPort(5432);
    _db.setUserName(_settings->value("Database/userName").toString());
    _db.setPassword(_settings->value("Database/password").toString());
    _db.setDatabaseName(_settings->value("Database/name").toString());
    if(_db.open())
    {
        return true;
    }
    else
    {
        qDebug() << "Couldn't open database!";
        return false;
    }
}

void DataBase::closeDataBase()
{
    _db.close();
}

bool DataBase::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "id SERIAL PRIMARY KEY, "
                            ID     " CHARACTER VARYING(255),"
                            BATCH_NUMBER      " CHARACTER VARYING(255),"
                            TIMESTAMP      " CHARACTER VARYING(255),"
                            OPERATOR      " CHARACTER VARYING(255),"
                            STATE      " CHARACTER VARYING(255)"
                        " )"
                    ))
    {
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        return true;
    }

    return false;
}

bool DataBase::insertIntoTable(const DutRecord &record)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " ID ", "
                                             BATCH_NUMBER ", "
                                             TIMESTAMP ", "
                                             OPERATOR ", "
                                             STATE " ) "
                  "VALUES (:deviceID, :batchNumber, :timeStamp, :operatorName, :state)");
    query.bindValue(":deviceID", record.id);
    query.bindValue(":batchNumber", record.batchNumber);
    query.bindValue(":timeStamp", record.timeStamp);
    query.bindValue(":operatorName", record.operatorName);
    query.bindValue(":state", record.state);

    if(!query.exec())
    {
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    }
    else
    {
        return true;
    }
    return false;
}
