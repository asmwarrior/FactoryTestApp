#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

TestFixtureWidget::TestFixtureWidget(const QSharedPointer<Session> &session, QWidget *parent) : QWidget(parent), _session(session)
{

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _groupBox = new QGroupBox("DUTs position");
    _groupBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
    _groupBox->setFixedSize(250, 500);
    QGridLayout* boxLayout = new QGridLayout;
    _groupBox->setLayout(boxLayout);
    mainLayout->addWidget(_groupBox);

    _buttonGroup = new QButtonGroup;
    _buttonGroup->setExclusive(false);

    for(auto & dut : _session->duts)
    {
        DutButton* button = new DutButton(dut.no, dut.testPanel, dut.pos);
        _buttons.push_back(button);
        _buttonGroup->addButton(button, dut.no);
    }

    int counter = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            boxLayout->addWidget(_buttons[counter], i, j);
            counter++;
        }
    }

//    for(int i = 1; i < 6; i++)
//    {

//        QGroupBox* testPanelBox = new QGroupBox("Test Panel " + QString().setNum(i));
//        _panelGroupBoxes.insert(i, testPanelBox);
//        testPanelBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
//        testPanelBox->setFixedWidth(250);
//        testPanelBox->setCheckable(true);
//        QHBoxLayout* boxLayout = new QHBoxLayout;
//        testPanelBox->setLayout(boxLayout);
//        mainLayout->addWidget(testPanelBox);

//        connect(testPanelBox, &QGroupBox::toggled, this, &TestFixtureWidget::setDutButtonsState);

//        for(int j = 1; j < 4; j++)
//        {
//            DutButton* button = new DutButton(i, j);
//            _buttons.push_back(button);
//            _buttonGroup->addButton(button);
//            _buttonGroup->setExclusive(true);
//            boxLayout->addWidget(button);
//        }
//    }
}

//void TestFixtureWidget::setDutButtonsState(bool toggled)
//{
//    auto panelNo = _panelGroupBoxes.key(dynamic_cast<QGroupBox*>(sender()));
//    for(auto & button : _buttons)
//    {
//        if(button->getTestPanelNo() == panelNo)
//        {
//            if(toggled)
//                button->setButtonState(DutButton::untested);
//            else
//                button->setButtonState(DutButton::inactive);
//        }
//    }
//}
