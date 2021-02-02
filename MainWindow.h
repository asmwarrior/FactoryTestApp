#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSettings>
#include <QComboBox>
#include <QListWidget>
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

    void logInfo(const QString &message);
    void logError(const QString &message);
    void logSuccess(const QString &message);

    QSharedPointer<QSettings> _settings;

    //--- GUI Elements ------------------------------------------------
    QComboBox* _selectDeviceModelBox;
    QListWidget *_logWidget;
};

#endif // MAINWINDOW_H
