#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void onSelectDeviceBoxCurrentTextChanged(const QString& text);

private:

    QComboBox* _selectDeviceModelBox;
};

#endif // MAINWINDOW_H
