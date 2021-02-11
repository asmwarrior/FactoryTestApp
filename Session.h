#pragma once

#include <QString>
#include <QList>

struct Dut
{
    enum State {unavaliable, untested, tested, warning};

    State state = untested;
    QString id;
    int no;
    int testPanel;
    int pos;
    bool checked = true;

    Dut(int no, int testPanel, int pos) {this->no = no; this->testPanel = testPanel; this->pos = pos;}
};

struct Session
{
    QString operatorName;
    int totalTested;
    int success;
    int failtures;

    QList<Dut> duts =
    {
        {1, 1, 1},
        {2, 1, 2},
        {3, 1, 3},

        {4, 2, 1},
        {5, 2, 2},
        {6, 2, 3},

        {7, 3, 1},
        {8, 3, 2},
        {9, 3, 3},

        {10, 4, 1},
        {11, 4, 2},
        {12, 4, 3},

        {13, 5, 1},
        {14, 5, 2},
        {15, 5, 3},
    };
};
