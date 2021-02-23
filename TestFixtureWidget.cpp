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
    _groupBox->setFixedSize(250, 450);
    QGridLayout* boxLayout = new QGridLayout;
    _groupBox->setLayout(boxLayout);
    mainLayout->addWidget(_groupBox);
    mainLayout->addSpacing(50);

    _buttonGroup = new QButtonGroup;
    _buttonGroup->setExclusive(false);

    for(auto & dut : _session->getDutList())
    {
        DutButton* button = new DutButton(dut->getNo(), dut->getTestPanel(), dut->getPos());
        _buttons.push_back(button);
        _buttonGroup->addButton(button, dut->getNo());
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

    connect(_buttonGroup, &QButtonGroup::idClicked, [=](int id)
    {
        _session->setCurrentDut(id);
        emit dutStateChanged();
    });
    connect(_buttonGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), [=](QAbstractButton *button, bool checked)
    {
        _session->getDutList()[(dynamic_cast<DutButton*>(button))->getNo() - 1]->setChecked(checked);
    });


    QHBoxLayout* selectionButtonsLayout = new QHBoxLayout;
    mainLayout->addLayout(selectionButtonsLayout);

    _selectAllButton = new QPushButton("Select all DUTs");
    _selectAllButton->setFixedHeight(40);
    selectionButtonsLayout->addWidget(_selectAllButton);
    connect(_selectAllButton, &QPushButton::clicked, [=]()
    {
        for (auto & button : _buttons)
        {
            button->setChecked(true);
        }
        emit dutStateChanged();
    });

    _reverseSelectionButton = new QPushButton("Reverse Selection");
    _reverseSelectionButton->setFixedHeight(40);
    selectionButtonsLayout->addWidget(_reverseSelectionButton);

    connect(_reverseSelectionButton, &QPushButton::clicked, [=]()
    {
        for (auto & button : _buttons)
        {
            button->setChecked(!button->isChecked());
        }
        emit dutStateChanged();
    });
}

void TestFixtureWidget::refreshButtonsState()
{
    for(int i = 0; i < _buttons.size(); i++)
    {
        _buttons.at(i)->setButtonState(_session->getDutList().at(i)->getState());
        _buttons.at(i)->setChecked(_session->getDutList().at(i)->isChecked());
    }
}
