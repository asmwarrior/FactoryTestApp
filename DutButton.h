#pragma once

#include <QWidget>
#include <QPushButton>
#include <QMap>

class DutButton : public QPushButton
{
    Q_OBJECT

public:

    enum State {unavaliable, inactive, untested, tested, warning};

    explicit DutButton(int no, int panelNo, int pos, QWidget *parent = nullptr);

    void setChecked(bool checked = true);
    void setTestPanelNo(int No);
    void setPos(int pos);
    int getTestPanelNo();
    int getPos();

signals:
    void clicked(bool checked);

public slots:

    void setButtonState(State state);

private:

    State _state;
    QMap<int, QString> _stateStyles;
    int _no;
    int _testPanelNo;
    int _pos;

};
