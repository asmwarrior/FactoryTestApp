#include "ActionHintWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

ActionHintWidget::ActionHintWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    _hintLabel = new QLabel;
    _hintLabel->setStyleSheet(_normalStyle);
    _hintLabel->setText(_hintActionTemplate.arg(""));
    mainLayout->addWidget(_hintLabel);
}

void ActionHintWidget::showNormalHint(const QString &text)
{
    _hintLabel->setStyleSheet(_normalStyle);
    _hintLabel->setText(_hintActionTemplate.arg(text));
}

void ActionHintWidget::showProgressHint(const QString &text)
{
    _hintLabel->setStyleSheet(_normalStyle);
    _hintLabel->setText(_hintProgressTemplate.arg(text));
}

