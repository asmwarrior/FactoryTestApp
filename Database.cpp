#include "Database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

void DataBase::connectToDataBase()
{

    if(!QFile("D:/Upwork/Capelon/" DATABASE_NAME).exists())
    {
        this->restoreDataBase();
    }
    else
    {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase())
    {
        return (this->createTable()) ? true : false;
    }
    else
    {
        qDebug() << "Couldn't open database!";
        return false;
    }
    return false;
}

bool DataBase::openDataBase()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setHostName(DATABASE_HOSTNAME);
    _db.setDatabaseName("D:/Upwork/Capelon/" DATABASE_NAME);
    if(_db.open())
    {
        return true;
    }
    else
    {
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
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_DEVICE_ID     " VARCHAR(255)    NOT NULL,"
                            TABLE_BIN      " BLOB            NOT NULL"
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
                                             TABLE_BIN " ) "
                  "VALUES (:DeviceID, :Bin)");
    query.bindValue(":DeviceID",        data[0].toString());
    query.bindValue(":Bin",         data[1].toByteArray());

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

bool DataBase::insertIntoTable(const QString &name, const QByteArray &pic)
{
    QVariantList data;
    data.append(name);
    data.append(pic);

    if(insertIntoTable(data))
        return true;
    else
        return false;
}
