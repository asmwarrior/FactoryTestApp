#pragma once

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>

#include "Session.h"
#include "DutButton.h"

class DutInfoWidget : public QWidget
{

    Q_OBJECT

public:

    DutInfoWidget(Session *session, QWidget* parent = nullptr);

public slots:

    void showDutInfo(int no);

private:

    //QSharedPointer<Session> _session;
    Session* _session;

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
