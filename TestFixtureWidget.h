#pragma once

#include <QWidget>
#include "DutButton.h"

class TestFixtureWidget : public QWidget
{

public:

    TestFixtureWidget(QWidget* parent = nullptr);

private:

    QList<DutButton*> _buttons;
    QButtonGroup* _buttonGroup;
};
