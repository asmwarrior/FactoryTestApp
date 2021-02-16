#include "Database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
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
    _db.setUserName(settings->value("Database/userName").toString());
    _db.setPassword(settings->value("Database/password").toString());
    _db.setDatabaseName(settings->value("Database/name").toString());
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
                            TABLE_DEVICE_ID     " CHARACTER VARYING(255),"
                            TABLE_INFO      " CHARACTER VARYING(255)"
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

bool DataBase::insertIntoTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( " TABLE_DEVICE_ID ", "
                                             TABLE_INFO " ) "
                  "VALUES (:DeviceID, :Info)");
    query.bindValue(":DeviceID",        data[0].toString());
    query.bindValue(":Info",         data[1].toString());

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

bool DataBase::insertIntoTable(const QString &name, const QString &info)
{
    QVariantList data;
    data.append(name);
    data.append(info);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}
