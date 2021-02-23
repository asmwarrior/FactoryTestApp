#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMap>

#include "Dut.h"

class DutButton : public QPushButton
{
    Q_OBJECT

public:

    explicit DutButton(int no, int panelNo, int pos, QWidget *parent = nullptr);

    void setChecked(bool checked = true);
    void setTestPanelNo(int No);
    void setPos(int pos);
    int getNo();
    int getTestPanelNo();
    int getPos();

signals:
    void clicked(bool checked);

public slots:

    void setButtonState(Dut::DutState state);

private:

    Dut::DutState _state;
    QMap<int, QString> _stateStyles;
    int _no;
    int _testPanelNo;
    int _pos;

};
