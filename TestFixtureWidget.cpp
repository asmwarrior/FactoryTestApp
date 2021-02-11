#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>

TestFixtureWidget::TestFixtureWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _buttonGroup = new QButtonGroup;
    for(int i = 1; i < 6; i++)
    {

        QGroupBox* testPanelBox = new QGroupBox("Test Panel " + QString().setNum(i));
        _panelGroupBoxes.insert(i, testPanelBox);
        testPanelBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
        testPanelBox->setFixedWidth(250);
        testPanelBox->setCheckable(true);
        QHBoxLayout* boxLayout = new QHBoxLayout;
        testPanelBox->setLayout(boxLayout);
        mainLayout->addWidget(testPanelBox);

        connect(testPanelBox, &QGroupBox::toggled, this, &TestFixtureWidget::setDutButtonsState);

        for(int j = 1; j < 4; j++)
        {
            DutButton* button = new DutButton(i, j);
            _buttons.push_back(button);
            _buttonGroup->addButton(button);
            _buttonGroup->setExclusive(true);
            boxLayout->addWidget(button);
        }
    }

//    _buttons[1]->setButtonState(DutButton::tested);
//    _buttons[2]->setButtonState(DutButton::tested);
//    _buttons[3]->setButtonState(DutButton::tested);
//    _buttons[4]->setButtonState(DutButton::tested);
//    _buttons[5]->setButtonState(DutButton::warning);
}

void TestFixtureWidget::setDutButtonsState(bool toggled)
{
    auto panelNo = _panelGroupBoxes.key(dynamic_cast<QGroupBox*>(sender()));
    for(auto & button : _buttons)
    {
        if(button->getTestPanelNo() == panelNo)
        {
            if(toggled)
                button->setButtonState(DutButton::untested);
            else
                button->setButtonState(DutButton::inactive);
        }
    }
}
