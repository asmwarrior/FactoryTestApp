#pragma once

#include <QWidget>
#include <QLabel>

#include "DutButton.h"

class SessionInfoWidget : public QWidget
{

    Q_OBJECT

public:

    SessionInfoWidget(QWidget* parent = nullptr);

public slots:


private:

    QLabel* _operatorName;
    QString _operatorNameTemplate = "<b>Operator name:</b> %1";

    QLabel* _totalTested;
    QString _totalTestedTemplate = "<b>Total tested:</b> %1";

    QLabel* _success;
    QString _successTemplate = "<b>Success:</b> %1";

    QLabel* _failtures;
    QString _failturesTemplate = "<b>Failtures:</b> %1";
};
