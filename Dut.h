#pragma once

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
    {"daliChecked", false},
    {"radioChecked", false},
    {"gnssChecked", false},
    {"rtcChecked", false},
    {"error", ""}
};

struct DutRecord
{
    QString runningNumber;
    QString id;
    QString no;
    QString cycleNo;
    QString method;
    QString batchNumber;
    QString timeStamp;
    QString operatorName;
    QString state;
    QString error;
};
