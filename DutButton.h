#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMap>

#include "Dut.h"

class DutButton : public QPushButton
{
    Q_OBJECT

public:

    explicit DutButton(int no, QWidget *parent = nullptr);

    void setChecked(bool checked = true);
    int getNo() {return _no;}

signals:
    void clicked(bool checked);

public slots:

    void setButtonState(int state);

private:

    int _no;
    int _state;
    QMap<int, QString> _stateStyles;
};
