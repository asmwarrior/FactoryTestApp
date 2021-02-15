#pragma once

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>

#include "AppComponent.h"
#include "DutButton.h"

class DutInfoWidget : public QWidget, public AppComponent
{

    Q_OBJECT

public:

    DutInfoWidget(QWidget* parent = nullptr);

public slots:

    void showDutInfo(int no);

private:

    QLabel* _slot;
    QString _slotTemplate = "<b>Slot:</b> %1";

    QLabel* _id;
    QString _idTemplate = "<b>Device ID:</b> %1";

    QLabel* _status;
    QString _statusTemplate = "<b>Status:</b> %1";

    QLabel* _errorDesc;
    QString _errorDescTemplate = "<b>Last error description:</b> %1";

    QLabel* _checkState;
};
