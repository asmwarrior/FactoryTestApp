#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QSharedPointer>

#include "Session.h"
#include "DutButton.h"

class TestFixtureWidget : public QWidget
{
    Q_OBJECT

public:

    TestFixtureWidget(const QSharedPointer<Session> &session, QWidget* parent = nullptr);

signals:

    void dutStateChanged();

private:

    QSharedPointer<Session> _session;

    QList<DutButton*> _buttons;
    QGroupBox* _groupBox;
    QButtonGroup* _buttonGroup;

    QPushButton* _selectAllButton;
    QPushButton* _reverseSelectionButton;
};
