#include "MainWindow.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* selectDeviceBoxLabel = new QLabel("Step 1. Select device model", this);
    _selectDeviceModelBox = new QComboBox(this);
    _selectDeviceModelBox->setFixedWidth(150);
    _selectDeviceModelBox->addItems({"Olc Nema", "Olc Zhaga"});
    connect(_selectDeviceModelBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onSelectDeviceBoxCurrentTextChanged(const QString&)));
    QHBoxLayout* selectDeviceLayout = new QHBoxLayout;
    selectDeviceLayout->addWidget(selectDeviceBoxLabel);
    selectDeviceLayout->addWidget(_selectDeviceModelBox);
    selectDeviceLayout->addStretch();

    mainLayout->addLayout(selectDeviceLayout);
    mainLayout->addStretch();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onSelectDeviceBoxCurrentTextChanged(const QString &text)
{
    qDebug() << text << "device selected";
}
