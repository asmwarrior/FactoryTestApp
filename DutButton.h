#pragma once

#include <QWidget>
#include <QPushButton>

class DutButton : public QPushButton
{
    Q_OBJECT

public:

    explicit DutButton(int panelNo, int pos, QWidget *parent = nullptr);

    void setChecked(bool checked = true);
    void setAlert(bool state = true);
    void setTestPanelNo(int No);
    void setPos(int pos);

signals:
    void clicked(bool checked);

public slots:

    void changeButtonState();

private:

    QString buttonStyle_;
    int _testPanelNo;
    int _pos;

};
