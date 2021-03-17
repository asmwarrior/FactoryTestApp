#pragma once

#include <QSettings>
#include <QTimer>
#include <QQueue>

#include "Logger.h"
#include "Dut.h"

class PrinterManager : public QObject
{
    Q_OBJECT

    public:


    explicit PrinterManager(const QSharedPointer<QSettings> &settings, QObject *parent = Q_NULLPTR);
    virtual ~PrinterManager();

    void setLogger(const QSharedPointer<Logger> &logger) {_logger = logger;}


public slots:

    void addLabel(DutRecord dutRecord);
    void sendLabeltoPrinter();
    bool isQueueEmpty() const;

signals:



private:

    QSharedPointer<QSettings> _settings;
    QSharedPointer<Logger> _logger;

    QTimer _timer;
    QQueue<DutRecord> _labelQueue;

    QString _fileName;
    QString _labelName;
    QString _quantity;
    QString _printer;
    QString _deviceName;
    QString _deviceRevision;
};
