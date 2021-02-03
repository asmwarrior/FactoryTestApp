#pragma once

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define DATABASE_HOSTNAME "TestDataBase"
#define DATABASE_NAME "test.db"

#define TABLE "TestTable"
#define TABLE_DEVICE_ID "DeviceID"
#define TABLE_BIN "Bin"

// First column contains Autoincrement ID

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void connectToDataBase();

private:
    QSqlDatabase _db;

private:

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

public slots:
    bool insertIntoTable(const QVariantList &data);
    bool insertIntoTable(const QString &name, const QByteArray &pic);
};
