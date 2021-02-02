#include "MainWindow.h"

#include <QDebug>

#include <QStandardPaths>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _settings = QSharedPointer<QSettings>::create(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/settings.ini", QSettings::IniFormat);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* selectDeviceBoxLabel = new QLabel("Step 1. Select device model", this);
    _selectDeviceModelBox = new QComboBox(this);
    _selectDeviceModelBox->setFixedWidth(150);
    _selectDeviceModelBox->addItems(_settings->value("Devices").toString().split("|"));
    connect(_selectDeviceModelBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onSelectDeviceBoxCurrentTextChanged(const QString&)));
    QHBoxLayout* selectDeviceLayout = new QHBoxLayout;
    selectDeviceLayout->addWidget(selectDeviceBoxLabel);
    selectDeviceLayout->addWidget(_selectDeviceModelBox);
    selectDeviceLayout->addStretch();

    mainLayout->addLayout(selectDeviceLayout);
    mainLayout->addStretch();

    _logWidget = new QListWidget(this);
    mainLayout->addWidget(_logWidget);

    logInfo("Info string");
    logError("error string");
    logSuccess("Success string");
}

MainWindow::~MainWindow()
{

}

void MainWindow::onSelectDeviceBoxCurrentTextChanged(const QString &text)
{
    qDebug() << text << "device selected";
}

void MainWindow::logInfo(const QString &message)
{
    _logWidget->addItem(message);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}

void MainWindow::logError(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::red);
    item->setForeground(Qt::white);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qCritical().noquote() << message;
}

void MainWindow::logSuccess(const QString &message)
{
    QListWidgetItem *item = new QListWidgetItem(message);

    item->setBackground(Qt::green);
    item->setForeground(Qt::black);
    _logWidget->addItem(item);
    _logWidget->scrollToBottom();
    qInfo().noquote() << message;
}
