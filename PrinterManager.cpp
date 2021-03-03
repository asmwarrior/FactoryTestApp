#include "PrinterManager.h"

#include <QDebug>

PrinterManager::PrinterManager(QSettings *settings, QObject *parent)
    : QObject(parent), _settings(settings)
{
    _fileName = _settings->value("workDirectory").toString() + "/" + _settings->value("Label/fileName").toString();
    _labelName = _settings->value("workDirectory").toString() + "/" + _settings->value("Label/labelName").toString();
    _quantity = _settings->value("Label/quantity").toString();
    _printer = _settings->value("Label/printer").toString();
    _deviceName = _settings->value("Label/deviceName").toString();
    _deviceRevision = _settings->value("Label/deviceRevision").toString();

    connect(&_timer, &QTimer::timeout, this, &PrinterManager::sendLabeltoPrinter);
    _timer.start(10000);
}

PrinterManager::~PrinterManager()
{
    _timer.stop();
}

void PrinterManager::addLabel(DutRecord dutRecord)
{
    _labelQueue.enqueue(dutRecord);
}

void PrinterManager::sendLabeltoPrinter()
{
    if(_labelQueue.isEmpty())
        return;

    QFile file(_fileName);

    file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    if (!file.isWritable())
        _logger->logError(QString("Cannot open label file '%1' for writing").arg(_fileName));

    file.write(QString("LABELNAME = \"%1\"\n").arg(_labelName).toLocal8Bit());
    file.write(QString("LABELQUANTITY = \"%1\"\n").arg(_quantity).toLocal8Bit());

    if (!_printer.isEmpty())
        file.write(QString("PRINTER = \"%1\"\n").arg(_printer).toLocal8Bit());

    file.write(QString("Name = \"%1\"\n").arg(_deviceName).toLocal8Bit());
    file.write(QString("Revision = \"%1\"\n").arg(_deviceRevision).toLocal8Bit());

    QDate date(QDate::currentDate());

    file.write(QString("Year = \"%1\"\n").arg(date.toString("yy")).toLocal8Bit());
    file.write(QString("Month = \"%1\"\n").arg(date.toString("MM")).toLocal8Bit());
    file.write(QString("Day = \"%1\"\n").arg(date.toString("dd")).toLocal8Bit());
    file.write(QString("Week = \"%1\"\n").arg(QString("%1").arg(date.weekNumber(), 2, 10, QChar('0'))).toLocal8Bit());

    auto currentRecord = _labelQueue.dequeue();
    file.write(QString("DID = \"%1\"\n").arg(currentRecord.id).toLocal8Bit());

    file.close();
}

