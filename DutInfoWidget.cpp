#include "DutInfoWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

DutInfoWidget::DutInfoWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QGroupBox* widgetBox = new QGroupBox("Selected DUT information");
    widgetBox->setFixedSize(350, 200);
    mainLayout->addWidget(widgetBox);
    mainLayout->addStretch();

    widgetBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");

    QVBoxLayout* labelsLayout = new QVBoxLayout;
    widgetBox->setLayout(labelsLayout);

    setStyleSheet("QLabel{color: #595959;}");

    _testPanel = new QLabel;
    _testPanel->setText(_testPanelTemplate.arg(1));
    labelsLayout->addWidget(_testPanel);

    _slot = new QLabel;
    _slot->setText(_slotTemplate.arg(2));
    labelsLayout->addWidget(_slot);

    _id = new QLabel;
    _id->setText(_idTemplate.arg("34567844"));
    labelsLayout->addWidget(_id);

    _status = new QLabel;
    _status->setText(_statusTemplate.arg("success"));
    labelsLayout->addWidget(_status);

    labelsLayout->addStretch();
}
