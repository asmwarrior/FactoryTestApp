#include "DutButton.h"

DutButton::DutButton(int panelNo, int pos, QWidget *parent) : QPushButton(parent), _testPanelNo(panelNo), _pos(pos)
{
    buttonStyle_ =  "QPushButton:checked{color: #ffffff; border-style:solid; border-width:0px; border-color:#00CE6E; background-color:#00b050; font-size:11pt}"
                            "QPushButton{color: #ffffff; border-style:solid; border-width:0px; border-color:#7f7f7f; background-color:#595959; font-size:11pt}";

    setCheckable(true);
    setChecked(false);
    setStyleSheet(buttonStyle_);
    setFixedSize(38, 38);
    setText(QString().setNum(pos));

    connect(this, &QPushButton::clicked, this, &DutButton::changeButtonState);
    connect(this, &QPushButton::clicked, this, &DutButton::clicked);
}

void DutButton::setChecked(bool checked)
{
    QPushButton::setChecked(checked);
    changeButtonState();
}

void DutButton::setAlert(bool state)
{
    if(state) setStyleSheet("color: #ffffff; border-style:solid; border-width:1px; border-color:#7f7f7f; background-color:#c05046; font-size:11pt");
    else setStyleSheet(buttonStyle_);
}

void DutButton::setTestPanelNo(int No)
{
    _testPanelNo = No;
}

void DutButton::setPos(int pos)
{
    _pos = pos;
}

void DutButton::changeButtonState()
{
    if(isChecked())
	{
        //setText("ON");
	}
    else
	{
        //setText("OFF");
        setStyleSheet(buttonStyle_);
	}
}
