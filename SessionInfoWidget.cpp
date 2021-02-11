#include "SessionInfoWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

SessionInfoWidget::SessionInfoWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QGroupBox* widgetBox = new QGroupBox("Session summury information");
    widgetBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
    widgetBox->setFixedSize(350, 200);
    mainLayout->addWidget(widgetBox);
    mainLayout->addStretch();

    QVBoxLayout* labelsLayout = new QVBoxLayout;
    widgetBox->setLayout(labelsLayout);

    setStyleSheet("QLabel{color: #595959;}");

    _operatorName = new QLabel;
    _operatorName->setText(_operatorNameTemplate.arg("Andrey Sokolov"));
    labelsLayout->addWidget(_operatorName);

    _totalTested = new QLabel;
    _totalTested->setText(_totalTestedTemplate.arg(650));
    labelsLayout->addWidget(_totalTested);

    _success = new QLabel;
    _success->setText(_successTemplate.arg(648));
    labelsLayout->addWidget(_success);

    _failtures = new QLabel;
    _failtures->setText(_failturesTemplate.arg(2));
    labelsLayout->addWidget(_failtures);

    labelsLayout->addStretch();
}
