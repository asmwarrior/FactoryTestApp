#include "SessionInfoWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

SessionInfoWidget::SessionInfoWidget(SessionManager* session, QWidget* parent) : QWidget(parent), _session(session)
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

    connect(_session, &SessionManager::sessionStatsChanged, this, &SessionInfoWidget::refresh);
}

void SessionInfoWidget::refresh()
{
    _startTime->setText(_startTimeTemplate.arg(_session->startTime()));
    _operatorName->setText(_operatorNameTemplate.arg(_session->operatorName()));
    _batchNumber->setText(_batchNumberTemplate.arg(_session->batchNumber()));
    _totalTested->setText(_totalTestedTemplate.arg(_session->successCount() + _session->failedCount()));
    _success->setText(_successTemplate.arg(_session->successCount()));
    _failtures->setText(_failturesTemplate.arg(_session->failedCount()));

    if(_session->batchInfo().length())
    {
        _batchInfo->setText(_session->batchInfo());
    }
}
