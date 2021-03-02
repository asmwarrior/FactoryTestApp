#pragma once

//#include <QObject>
//#include <QStringList>
//#include <QDebug>

#include <QVariant>

enum DutState {inactive, untested, tested, warning};

using Dut = QMap<QString, QVariant>;

static Dut dutTemplate {
    {"state", DutState::inactive},
    {"id", ""},
    {"no", 0},
    {"checked", false},
    {"voltageChecked", false},
    {"accelChecked", false},
    {"lightSensChecked", false},
    {"daliChecked", false}
};

struct DutRecord
{
    QString id;
    QString batchNumber;
    QString timeStamp;
    QString operatorName;
    QString state;
};
