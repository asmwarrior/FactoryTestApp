#include "DutInfoWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

DutInfoWidget::DutInfoWidget(Session *session, QWidget *parent) : QWidget(parent), _session(session)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QGroupBox* widgetBox = new QGroupBox("DUT information");
    widgetBox->setFixedSize(350, 200);
    mainLayout->addWidget(widgetBox);
    mainLayout->addStretch();

    widgetBox->setStyleSheet("QGroupBox{color: #595959; font-size:10pt; font-weight: bold;}");

    QVBoxLayout* labelsLayout = new QVBoxLayout;
    widgetBox->setLayout(labelsLayout);

    setStyleSheet("QLabel{color: #595959;}");


    _slot = new QLabel;
    _slot->setText(_slotTemplate.arg(""));
    labelsLayout->addWidget(_slot);

    _id = new QLabel;
    _id->setText(_idTemplate.arg(""));
    labelsLayout->addWidget(_id);

    _status = new QLabel;
    _status->setText(_statusTemplate.arg(""));
    labelsLayout->addWidget(_status);

    _errorDesc = new QLabel;
    labelsLayout->addWidget(_errorDesc);

    _checkState = new QLabel;
    labelsLayout->addWidget(_checkState);

    labelsLayout->addStretch();
}

void DutInfoWidget::showDutInfo(int no)
{
    if(no < 1)
        return;

    _slot->setText(_slotTemplate.arg(no));
    _id->setText(_idTemplate.arg(_session->getDutsList()[no - 1].id));

    QString stateDescription;
    switch (_session->getDutsList()[no - 1].state)
    {
        case Dut::unavaliable:
        stateDescription = "The device is not avaliable now.";
        break;

        case Dut::untested:
        stateDescription = "The device is not tested yet.</p>";
        break;

        case Dut::tested:
        stateDescription = "The device passes testing successfully.";
        break;

        case Dut::warning:
        stateDescription = "An error occures during the testing.";
        break;
    }


    _status->setText(_statusTemplate.arg(stateDescription));

    if(!_session->getDutsList()[no - 1].lastErrorDescription.isEmpty())
    {
        _errorDesc->setText(_errorDescTemplate.arg(_session->getDutsList()[no - 1].lastErrorDescription));
    }

    else
    {
        _errorDesc->setText("");
    }

    if(_session->getDutsList()[no - 1].checked)
    {
        _checkState->setText("CHECKED for a further testing.");
    }

    else
    {
        _checkState->setText("NOT CHECKED for a further testing.");
    }
}
