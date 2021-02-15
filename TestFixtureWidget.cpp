#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

TestFixtureWidget::TestFixtureWidget(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _groupBox = new QGroupBox("DUTs position");
    _groupBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
    _groupBox->setFixedSize(250, 500);
    QGridLayout* boxLayout = new QGridLayout;
    _groupBox->setLayout(boxLayout);
    mainLayout->addWidget(_groupBox);
    mainLayout->addSpacing(50);

    _buttonGroup = new QButtonGroup;
    _buttonGroup->setExclusive(false);

    for(auto & dut : dutList)
    {
        DutButton* button = new DutButton(dut->no, dut->testPanel, dut->pos);
        _buttons.push_back(button);
        _buttonGroup->addButton(button, dut->no);
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
        session->setCurrentDut(id);
        emit dutStateChanged();
    });
    connect(_buttonGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), [=](QAbstractButton *button, bool checked)
    {
        dutList[(dynamic_cast<DutButton*>(button))->getNo() - 1]->checked = checked;
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
