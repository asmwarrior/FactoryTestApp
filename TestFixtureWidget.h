#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include <QSharedPointer>

#include "Session.h"
#include "DutButton.h"

class TestFixtureWidget : public QWidget
{

public:

    TestFixtureWidget(const QSharedPointer<Session> &session, QWidget* parent = nullptr);

private slots:

//    void setDutButtonsState(bool toggled);

private:

    QSharedPointer<Session> _session;

    QList<DutButton*> _buttons;
    QGroupBox* _groupBox;
    QButtonGroup* _buttonGroup;
};
