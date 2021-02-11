#pragma once

#include <QString>
#include <QMap>

struct Dut
{
    enum State {unavaliable, inactive, untested, tested, warning};

    State state;
    QString id;
};

struct TestPanel
{
    QMap<int, Dut> duts;
    bool active = true;
};

struct Session
{
    QString operatorName;
    int totalTested;
    int success;
    int failtures;

    QMap<int, TestPanel> testPanels;
};
