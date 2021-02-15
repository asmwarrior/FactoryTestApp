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

    QGroupBox* widgetBox = new QGroupBox("Session summary");
    widgetBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");
    widgetBox->setFixedSize(350, 200);
    mainLayout->addWidget(widgetBox);
    mainLayout->addStretch();

    QVBoxLayout* labelsLayout = new QVBoxLayout;
    widgetBox->setLayout(labelsLayout);

    setStyleSheet("QLabel{color: #595959;}");

    _startTime = new QLabel;
    labelsLayout->addWidget(_startTime);

    _operatorName = new QLabel;
    labelsLayout->addWidget(_operatorName);

    _batchNumber = new QLabel;
    labelsLayout->addWidget(_batchNumber);

    _totalTested = new QLabel;
    labelsLayout->addWidget(_totalTested);

    _success = new QLabel;
    labelsLayout->addWidget(_success);

    _failtures = new QLabel;
    labelsLayout->addWidget(_failtures);

    labelsLayout->addStretch();

    _batchInfo = new QLabel;
    labelsLayout->addWidget(_batchInfo);

    labelsLayout->addStretch();
}

void SessionInfoWidget::update()
{
    if(session->isStarted())
    {
        _startTime->setText(_startTimeTemplate.arg(session->getStartTime()));
        _operatorName->setText(_operatorNameTemplate.arg(session->getOperatorName()));
        _batchNumber->setText(_batchNumberTemplate.arg(session->getBatchNumber()));
        _totalTested->setText(_totalTestedTemplate.arg(session->getTotalTested()));
        _success->setText(_successTemplate.arg(session->getSuccessCount()));
        _failtures->setText(_failturesTemplate.arg(session->getFailedCount()));

        if(session->getBatchInfo().length())
        {
            _batchInfo->setText(session->getBatchInfo());
        }
    }

    else
    {
        _startTime->clear();
        _operatorName->clear();
        _batchNumber->clear();
        _totalTested->clear();
        _success->clear();
        _failtures->clear();
        _batchInfo->clear();
    }
}
