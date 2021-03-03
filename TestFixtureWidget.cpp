#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

TestFixtureWidget::TestFixtureWidget(SessionManager* session, QWidget* parent) : QWidget(parent), _session(session)
{

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* _title = new QLabel("<b>Step 3.</b> Select DUTs");
    mainLayout->addWidget(_title);
    _groupBox = new QGroupBox();
    _groupBox->setStyleSheet("QGroupBox{font-weight: bold;}");
    _groupBox->setFixedSize(450, 250);
    QGridLayout* boxLayout = new QGridLayout;
    _groupBox->setLayout(boxLayout);
    mainLayout->addWidget(_groupBox);
    mainLayout->addSpacing(50);

    _buttonGroup = new QButtonGroup;
    _buttonGroup->setExclusive(false);

    for(int i = 1; i < 16; i++)
    {
        DutButton* button = new DutButton(i, this);
        button->setText(QString().setNum(i));
        _buttons.push_back(button);
        _buttonGroup->addButton(button, i);
    }

//    int counter = 0;
//    for(int i = 0; i < 5; i++)
//    {
//        for(int j = 0; j < 3; j++)
//        {
//            boxLayout->addWidget(_buttons[counter], i, j);
//            counter++;
//        }
//    }

    boxLayout->addWidget(_buttons[0], 2, 0);
    boxLayout->addWidget(_buttons[1], 1, 0);
    boxLayout->addWidget(_buttons[2], 0, 0);

    boxLayout->addWidget(_buttons[3], 2, 1);
    boxLayout->addWidget(_buttons[4], 1, 1);
    boxLayout->addWidget(_buttons[5], 0, 1);

    boxLayout->addWidget(_buttons[6], 2, 2);
    boxLayout->addWidget(_buttons[7], 1, 2);
    boxLayout->addWidget(_buttons[8], 0, 2);

    boxLayout->addWidget(_buttons[9], 2, 3);
    boxLayout->addWidget(_buttons[10], 1, 3);
    boxLayout->addWidget(_buttons[11], 0, 3);

    boxLayout->addWidget(_buttons[12], 2, 4);
    boxLayout->addWidget(_buttons[13], 1, 4);
    boxLayout->addWidget(_buttons[14], 0, 4);

    connect(_buttonGroup, &QButtonGroup::idClicked, [=](int id)
    {
//        _session->setCurrentDut(id);
        emit dutClicked(id, _buttonGroup->button(id)->isChecked());
    });
    connect(_buttonGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), [=](QAbstractButton *button, bool checked)
    {
        //emit dutChecked((dynamic_cast<DutButton*>(button))->getNo(), checked);
    });


    QHBoxLayout* selectionButtonsLayout = new QHBoxLayout;
    mainLayout->addLayout(selectionButtonsLayout);

    _selectAllButton = new QPushButton("Select all DUTs");
    _selectAllButton->setFixedSize(120, 40);
    selectionButtonsLayout->addWidget(_selectAllButton);
    connect(_selectAllButton, &QPushButton::clicked, [=]()
    {
        for (auto & button : _buttons)
        {
            button->setChecked(true);
        }
        //emit dutStateChanged();
    });

    _reverseSelectionButton = new QPushButton("Reverse Selection");
    _reverseSelectionButton->setFixedSize(120, 40);
    selectionButtonsLayout->addWidget(_reverseSelectionButton);
    selectionButtonsLayout->addStretch();

    connect(_reverseSelectionButton, &QPushButton::clicked, [=]()
    {
        for (auto & button : _buttons)
        {
            button->setChecked(!button->isChecked());
        }
        //emit dutStateChanged();
    });
}

void TestFixtureWidget::refreshButtonsState()
{
//    for(int i = 0; i < _buttons.size(); i++)
//    {
//        _buttons.at(i)->setButtonState(_session->getDutList().at(i)->getState());
//        _buttons.at(i)->setChecked(_session->getDutList().at(i)->isChecked());
    //    }
}

void TestFixtureWidget::reset()
{
    for (auto & button : _buttons)
    {
        (dynamic_cast<DutButton*>(button))->setButtonState(DutState::inactive);
        (dynamic_cast<DutButton*>(button))->setChecked(false);
    }
}

void TestFixtureWidget::refreshButtonState(Dut dut)
{
    _buttons.at(dut["no"].toInt() - 1)->setButtonState(dut["state"].toInt());
    _buttons.at(dut["no"].toInt() - 1)->setChecked(dut["checked"].toBool());
}
