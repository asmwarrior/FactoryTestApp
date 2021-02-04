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

#define TABLE "TestTable"
#define TABLE_DEVICE_ID "DeviceID"
#define TABLE_INFO "Info"

class DataBase : public QObject
{
    Q_OBJECT

public:

    explicit DataBase(const QSharedPointer<QSettings>& settings, QObject *parent = nullptr);
    ~DataBase();

    void connectToDataBase();

public slots:

    bool insertIntoTable(const QVariantList &data);
    bool insertIntoTable(const QString &name, const QString &info);

private:

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

    QSqlDatabase _db;
    QSharedPointer<QSettings> _settings;
};
