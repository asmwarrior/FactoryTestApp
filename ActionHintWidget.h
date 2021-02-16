#pragma once

#include <QWidget>
#include <QLabel>

class ActionHintWidget : public QWidget
{
    Q_OBJECT

public:

    ActionHintWidget(QWidget* parent = nullptr);

    void showNormalHint(const QString& text);

private:

    QLabel* _hintLabel;
    QString _hintLabelTemplate = "NEXT ACTION: %1";
    QString _normalStyle = "color: #689F38; font-size:10pt; font-weight: bold;";
};
