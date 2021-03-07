#include "DutButton.h"

DutButton::DutButton(int no, QWidget *parent) : QPushButton(parent), _no(no)
{
    _stateStyles =
    {
        {DutState::inactive, "QPushButton{color: #9E9E9E; border-style:solid; border-width:3px; border-color:#F0F0F0; background-color:#F0F0F0; font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::untested, "QPushButton{color: #404040; border-style:solid; border-width:3px; border-color:#F0F0F0; background-image:url(:/icons/dut_untested); font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::tested, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-image:url(:/icons/dut_tested); font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"},

        {DutState::warning, "QPushButton{color: #ffffff; border-style:solid; border-width:3px; border-color:#F0F0F0; background-image:url(:/icons/dut_warning); font-size:11pt;}"
        "QPushButton:checked{border-width:3px; border-color:#FFB74D;}"}
    };

    setCheckable(true);
    setChecked(false);
    setButtonState(DutState::inactive);
    setFixedSize(54, 54);
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
