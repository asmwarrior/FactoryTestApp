#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include "RailtestClient.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void onSelectDeviceBoxCurrentTextChanged(const QString& text);
    void downloadRailtest();
    void testRadio(RailtestClient *rail);
    void startFullCycleTesting();

private:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

    void jLinkScript(const QString &fileName);

    QSharedPointer<QSettings> _settings;

    //--- GUI Elements ------------------------------------------------
    QComboBox* _selectDeviceModelBox;
    QPushButton* _startFullCycleTesting;
    QListWidget *_logWidget;
};

#endif // MAINWINDOW_H
