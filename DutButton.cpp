#include "DutButton.h"

DutButton::DutButton(int panelNo, int pos, QWidget *parent) : QPushButton(parent), _testPanelNo(panelNo), _pos(pos)
{
    _stateStyles =
    {
        {unavaliable, "QPushButton{color: #9E9E9E; border-style:solid; border-width:1px; border-color:#9E9E9E; background-color:#ffffff; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FDD835;}"},

        {inactive, "QPushButton{color: #9E9E9E; border-style:solid; border-width:1px; border-color:#9E9E9E; background-color:#ffffff; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FDD835;}"},

        {untested, "QPushButton{color: #ffffff; border-style:solid; border-width:0px; border-color:#7f7f7f; background-color:#595959; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FDD835;}"},

        {tested, "QPushButton{color: #ffffff; border-style:solid; border-width:0px; border-color:#00CE6E; background-color:#00b050; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FDD835;}"},

        {warning, "QPushButton{color: #ffffff; border-style:solid; border-width:1px; border-color:#7f7f7f; background-color:#c05046; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FDD835;}"}
    };

    setCheckable(true);
    setChecked(false);
    setButtonState(untested);
    setFixedSize(38, 38);
    setText(QString().setNum(pos));

    connect(this, &QPushButton::toggled, this, &DutButton::onToggled);
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

void DutButton::setButtonState(State state)
{
    _state = state;
    setStyleSheet(_stateStyles[_state]);

    if(isChecked())
	{
	}
    else
	{
    }
}

void DutButton::onToggled(bool state)
{
    if(state)
    {
        setFixedSize(44, 44);
    }

    else
    {
        setFixedSize(38, 38);
    }
}

void DutButton::onClicked(bool state)
{
}
