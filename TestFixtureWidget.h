#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QSharedPointer>

#include "DutButton.h"
#include "SessionManager.h"

class TestFixtureWidget : public QWidget
{
    Q_OBJECT

public:

    TestFixtureWidget(SessionManager* session, QWidget* parent = nullptr);

public slots:

    void refreshButtonsState();

signals:

    void dutStateChanged();

private:

    SessionManager* _session;

    QList<DutButton*> _buttons;
    QGroupBox* _groupBox;
    QButtonGroup* _buttonGroup;

    QPushButton* _selectAllButton;
    QPushButton* _reverseSelectionButton;
};
