#pragma once

#include <QString>
#include <QList>
#include <QObject>

struct Dut
{
    enum State {unavaliable, untested, tested, warning};

    State state = untested;
    QString id;
    int no;
    int testPanel;
    int pos;
    QString lastErrorDescription;
    bool checked = true;

    Dut(int no, int testPanel, int pos) {this->no = no; this->testPanel = testPanel; this->pos = pos;}
};

class Session : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString operatorName WRITE setOperatorName READ getOperatorName)
    Q_PROPERTY(int currentDut WRITE setCurrentDut READ getCurrentDut)
    Q_PROPERTY(QList<Dut> duts READ getDutsList)

public:

    explicit Session(QObject* parent = nullptr) : QObject(parent) {}

    QString getOperatorName() const
    {
        return m_operatorName;
    }

    QList<Dut> getDutsList() const
    {
        return m_duts;
    }

    int getCurrentDut() const
    {
        return m_currentDut;
    }

public slots:
    void setOperatorName(QString operatorName)
    {
        m_operatorName = operatorName;
    }

    void setCurrentDut(int currentDut)
    {
        m_currentDut = currentDut;
    }

private:

    QString m_operatorName = "Andrey Sokolov";
    int m_currentDut;
    QList<Dut> m_duts =
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
