#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include <QSqlTableModel>
#include <QModelIndex>

#include "Database.h"
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
    void initDali(RailtestClient *rail);
    void testRadio(RailtestClient *rail);
    void testAccelerometer(RailtestClient *rail);
    void testLightSensor(RailtestClient *rail);
    void testDALI(RailtestClient *rail);
    void testGNSS(RailtestClient *rail);
    void startFullCycleTesting();
    void onRfReplyReceived(QString id, QVariantMap params);

private:

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

    void jLinkScript(const QString &fileName);

    QSharedPointer<QSettings> _settings;
    DataBase *_db;
    QSqlTableModel  *model;

    //--- GUI Elements ------------------------------------------------
    QComboBox* _selectDeviceModelBox;
    QPushButton* _startFullCycleTesting;
    QListWidget *_logWidget;

    int _rfRSSI;
    int _rfCount;
};

#endif // MAINWINDOW_H
