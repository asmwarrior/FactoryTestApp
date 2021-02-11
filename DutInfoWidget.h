#pragma once

#include <QWidget>
#include <QLabel>
#include "DutButton.h"

class DutInfoWidget : public QWidget
{

public:

    DutInfoWidget(QWidget* parent = nullptr);

private:

    QLabel* _testPanel;
    QString _testPanelTemplate = "<b>Test panel:</b> %1";

    QLabel* _slot;
    QString _slotTemplate = "<b>Slot:</b> %1";

    QLabel* _id;
    QString _idTemplate = "<b>Device ID:</b> %1";

    QLabel* _status;
    QString _statusTemplate = "<b>Status:</b> %1";
};
