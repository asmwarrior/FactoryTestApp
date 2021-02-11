#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

TestFixtureWidget::TestFixtureWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _buttonGroup = new QButtonGroup;
    for(int i = 1; i < 6; i++)
    {
        QGroupBox* testPanelBox = new QGroupBox("Test Panel " + QString().setNum(i));
        testPanelBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
        testPanelBox->setFixedWidth(250);
        testPanelBox->setCheckable(true);
        QHBoxLayout* boxLayout = new QHBoxLayout;
        testPanelBox->setLayout(boxLayout);
        mainLayout->addWidget(testPanelBox);
        for(int j = 1; j < 4; j++)
        {
            DutButton* button = new DutButton(i, j);
            _buttonGroup->addButton(button);
            _buttonGroup->setExclusive(true);
            boxLayout->addWidget(button);
        }
    }
}
