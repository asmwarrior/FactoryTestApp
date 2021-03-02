#pragma once

#include <QWidget>
#include <QLabel>

#include "DutButton.h"
#include "SessionManager.h"

class SessionInfoWidget : public QWidget
{

    Q_OBJECT

public:

    SessionInfoWidget(SessionManager* session, QWidget* parent = nullptr);

public slots:

    void refresh();


private:

    SessionManager* _session;

    QLabel* _startTime;
    QString _startTimeTemplate = "<b>Started at:</b> %1";

    QLabel* _operatorName;
    QString _operatorNameTemplate = "<b>Operator name:</b> %1";

    QLabel* _batchNumber;
    QString _batchNumberTemplate = "<b>Batch number:</b> %1";

    QLabel* _totalTested;
    QString _totalTestedTemplate = "<b>Total tested:</b> %1";

    QLabel* _success;
    QString _successTemplate = "<b>Success:</b> %1";

    QLabel* _failtures;
    QString _failturesTemplate = "<b>Failtures:</b> %1";

    QLabel* _batchInfo;
};
