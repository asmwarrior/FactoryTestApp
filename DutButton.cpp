#include "DutButton.h"

DutButton::DutButton(int no, int panelNo, int pos, QWidget *parent) : QPushButton(parent), _no(no), _testPanelNo(panelNo), _pos(pos)
{
    _stateStyles =
    {
        {unavaliable, "QPushButton{color: #9E9E9E; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#ffffff; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#F57F17;}"},

        {inactive, "QPushButton{color: #9E9E9E; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#ffffff; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#F57F17;}"},

        {untested, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#595959; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#F57F17;}"},

        {tested, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#00b050; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#F57F17;}"},

        {warning, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#c05046; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#F57F17;}"}
    };

    setCheckable(true);
    setChecked(false);
    setButtonState(warning);
    setFixedSize(44, 44);
    setText(QString().setNum(_no));
}

void DutButton::setChecked(bool checked)
{
    QPushButton::setChecked(checked);
}

void DutButton::setTestPanelNo(int No)
{
    _testPanelNo = No;
}

void DutButton::setPos(int pos)
{
    _pos = pos;
}

int DutButton::getTestPanelNo()
{
    return _testPanelNo;
}

int DutButton::getPos()
{
    return _pos;
}

void DutButton::setButtonState(State state)
{
    _state = state;
    setStyleSheet(_stateStyles[_state]);
}
