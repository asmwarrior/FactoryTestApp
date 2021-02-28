#include "DutInfoWidget.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>

DutInfoWidget::DutInfoWidget(SessionManager *session, QWidget *parent) : QWidget(parent), _session(session)
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

    for (int no = 1; no < 16; no++)
    {
        _duts.insert(no, dutTemplate);
    }
}

void DutInfoWidget::showDutInfo(int no)
{
    if(no < 1)
        return;

    auto dut = _duts[no];

    _slot->setText(_slotTemplate.arg(no));
    _id->setText(_idTemplate.arg(dut["id"].toString()));

    QString stateDescription;
    switch (dut["state"].toInt())
    {
        case DutState::inactive:
        stateDescription = "The device is not avaliable now.";
        break;

        case DutState::untested:
        stateDescription = "The device is not tested yet.</p>";
        break;

        case DutState::tested:
        stateDescription = "The device passes testing successfully.";
        break;

        case DutState::warning:
        stateDescription = "An error occures during the testing.";
        break;
    }


    _status->setText(_statusTemplate.arg(stateDescription));

//    if(!dut->getErrorList().isEmpty())
//    {
//        _errorDesc->setText(_errorDescTemplate.arg(dut->getErrorList().last()));
//    }

//    else
//    {
//        _errorDesc->setText("");
//    }

    if(dut["checked"].toBool())
    {
        _checkState->setText("CHECKED for a further testing.");
    }

    else
    {
        _checkState->setText("NOT CHECKED for a further testing.");
        }
}

void DutInfoWidget::updateDut(Dut dut)
{
    _duts[dut["no"].toInt()] = dut;
}

void DutInfoWidget::setDutChecked(int no, bool checked)
{
    _duts[no]["checked"] = checked;
}
