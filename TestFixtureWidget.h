#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QSharedPointer>

#include "AppComponent.h"
#include "DutButton.h"

class TestFixtureWidget : public QWidget, public AppComponent
{
    Q_OBJECT

public:

    TestFixtureWidget(QWidget* parent = nullptr);

signals:

    void dutStateChanged();

private:

    QList<DutButton*> _buttons;
    QGroupBox* _groupBox;
    QButtonGroup* _buttonGroup;

    QPushButton* _selectAllButton;
    QPushButton* _reverseSelectionButton;
};
