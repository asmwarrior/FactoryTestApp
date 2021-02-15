#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QSqlTableModel>
#include <QModelIndex>

#include "AppComponent.h"
#include "JLinkManager.h"
#include "Database.h"
#include "RailtestClient.h"
#include "TestFixtureWidget.h"
#include "SessionInfoWidget.h"
#include "DutInfoWidget.h"

class MainWindow : public QWidget, public AppComponent
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() Q_DECL_OVERRIDE;

public slots:

    void startFullCycleTesting();

private:

    void setCurrentJLinkIndex(int index);
    int getCurrentJLinkIndex();

    QList<QThread*> _threads;
    QList<JLinkManager*> _JLinksList;
    QList<RailtestClient*> _railTestClientsList;

    DataBase *_db;
    QSqlTableModel  *model;

    //--- GUI Elements ------------------------------------------------

    QLabel* _headerLabel;
    QLineEdit* _operatorNameEdit;
    QLineEdit* _batchNumberEdit;
    QLineEdit* _batchInfoEdit;
    QPushButton* _newSessionButton;
    QPushButton* _finishSessionButton;
    QComboBox* _selectMetodBox;
    QListWidget* _testFunctionsListWidget;
    QPushButton* _startFullCycleTestingButton;
    QPushButton* _startSelectedTestButton;
    QListWidget* _logWidget;
    TestFixtureWidget* _testFixtureWidget;
    SessionInfoWidget* _sessionInfoWidget;
    DutInfoWidget* _dutInfoWidget;

    QLabel* _actionHintLabel;
    QString _actionHintStartText = "NEXT ACTION: Place DUTs into the test fixture, enter information for the Step 1 and start test session";
    QString _actionHintChooseMethod = "NEXT ACTION: Choose a test method (Step 2), select DUTs and run full cycle testing (or single step testing) for the selected DUTs";
};

#endif // MAINWINDOW_H
