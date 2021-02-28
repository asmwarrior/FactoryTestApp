#include "DutButton.h"

DutButton::DutButton(int no, QWidget *parent) : QPushButton(parent), _no(no)
{
    _stateStyles =
    {
        {DutState::inactive, "QPushButton{color: #9E9E9E; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#ffffff; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::untested, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#9E9E9E; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::tested, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#00b050; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::warning, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#c05046; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"}
    };

    setCheckable(true);
    setChecked(false);
    setButtonState(DutState::inactive);
    setFixedSize(44, 44);
}

void DutButton::setChecked(bool checked)
{
    QPushButton::setChecked(checked);
}

void DutButton::setButtonState(int state)
{
    _state = state;
    setStyleSheet(_stateStyles[_state]);
}
