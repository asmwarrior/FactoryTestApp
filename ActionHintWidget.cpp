#include "ActionHintWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

ActionHintWidget::ActionHintWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _hintLabel = new QLabel;
    _hintLabel->setStyleSheet(_normalStyle);
    _hintLabel->setText(_hintLabelTemplate.arg(""));
    mainLayout->addWidget(_hintLabel);
}

void ActionHintWidget::showNormalHint(const QString &text)
{
    _hintLabel->setStyleSheet(_normalStyle);
    _hintLabel->setText(_hintLabelTemplate.arg(text));
}

