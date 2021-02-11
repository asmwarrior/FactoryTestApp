#pragma once

#include <QWidget>
#include <QGroupBox>
#include <QButtonGroup>
#include "DutButton.h"

class TestFixtureWidget : public QWidget
{

public:

    TestFixtureWidget(QWidget* parent = nullptr);

private slots:

    void setDutButtonsState(bool toggled);

private:

    QList<DutButton*> _buttons;
    QMap<int, QGroupBox*> _panelGroupBoxes;
    QButtonGroup* _buttonGroup;
};
