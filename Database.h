#pragma once

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QSettings>
#include <QSharedPointer>
#include <QDebug>

#include "Dut.h"

#define TABLE "TestTable"
#define ID "deviceID"
#define BATCH_NUMBER "batchNumber"
#define TIMESTAMP "timeStamp"
#define OPERATOR "operatorName"
#define STATE "state"

class DataBase : public QObject
{
    Q_OBJECT

public:

    explicit DataBase(const QSharedPointer<QSettings> &settings, QObject *parent = nullptr);
    ~DataBase();

    void connectToDataBase();

public slots:

    bool insertIntoTable(const DutRecord& record);
    bool createTable();

private:

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();

    QSqlDatabase _db;
    QSharedPointer<QSettings> _settings;
};
