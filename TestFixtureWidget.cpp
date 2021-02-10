#include "TestFixtureWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

TestFixtureWidget::TestFixtureWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    for(int i = 1; i < 6; i++)
    {
        QGroupBox* testPanelBox = new QGroupBox("Test Panel " + QString().setNum(i));
        testPanelBox->setFixedWidth(200);
        testPanelBox->setCheckable(true);
        QHBoxLayout* boxLayout = new QHBoxLayout;
        testPanelBox->setLayout(boxLayout);
        mainLayout->addWidget(testPanelBox);
        for(int j = 1; j < 4; j++)
        {
            DutButton* button = new DutButton(i, j);
            boxLayout->addWidget(button);
        }
    }
}
