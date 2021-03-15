#include "PrinterManager.h"

#include <QDebug>
#include <QProcess>

PrinterManager::PrinterManager(QSettings *settings, QObject *parent)
    : QObject(parent), _settings(settings)
{
    _fileName = _settings->value("workDirectory").toString() + "/" + _settings->value("Label/fileName").toString();
    _labelName = _settings->value("Label/labelName").toString();
    _quantity = _settings->value("Label/quantity").toString();
    _printer = _settings->value("Label/printer").toString();
    _deviceName = _settings->value("Label/deviceName").toString();
    _deviceRevision = _settings->value("Label/deviceRevision").toString();

    connect(&_timer, &QTimer::timeout, this, &PrinterManager::sendLabeltoPrinter);
    _timer.start(10000);

//    DutRecord record;
//    record.id = "FF121234AB3456CD";
//    record.method = "OLC Zhaga ECO";
//    record.cycleNo = "1";
//    record.no = "7";
//    record.runningNumber = "0072";
//    addLabel(record);
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

    auto currentRecord = _labelQueue.dequeue();

    file.write(QString("LABELNAME = \"%1\"\n").arg(_labelName).toLocal8Bit());
    file.write(QString("LABELQUANTITY = \"%1\"\n").arg(_quantity).toLocal8Bit());

    if (!_printer.isEmpty())
        file.write(QString("PRINTER = \"%1\"\n").arg(_printer).toLocal8Bit());

    file.write(QString("Name = \"%1\"\n").arg(currentRecord.method).toLocal8Bit());
    file.write(QString("Revision = \"%1\"\n").arg(_deviceRevision).toLocal8Bit());

    file.write(QString("cycleNo = \"%1\"\n").arg(currentRecord.cycleNo).toLocal8Bit());
    file.write(QString("slot = \"%1\"\n").arg(currentRecord.no).toLocal8Bit());
    file.write(QString("runningNumber = \"%1\"\n").arg(currentRecord.runningNumber).toLocal8Bit());

    file.write(QString("NID = \"%1\"\n").arg(currentRecord.id).toLocal8Bit());

    file.close();
}

