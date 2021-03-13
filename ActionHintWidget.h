#pragma once

#include <QWidget>
#include <QLabel>

class ActionHintWidget : public QWidget
{
    Q_OBJECT

public:

    ActionHintWidget(QWidget* parent = nullptr);

public slots:


    void showNormalHint(const QString& text);
    void showProgressHint(const QString& text);

private:

    QLabel* _hintLabel;
    QString _hintActionTemplate = "NEXT ACTION: %1";
    QString _hintProgressTemplate = "NOW IN PROGRESS: %1";
    QString _normalStyle = "color: #689F38; font-size:10pt; font-weight: bold;";
};
