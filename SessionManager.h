#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QList>

#include "Dut.h"

class SessionManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString operatorName WRITE setOperatorName READ getOperatorName)
    Q_PROPERTY(QString startTime WRITE setStartTime READ getStartTime)
    Q_PROPERTY(QString batchNumber WRITE setBatchNumber READ getBatchNumber)
    Q_PROPERTY(QString batchInfo WRITE setBatchInfo READ getBatchInfo)
    Q_PROPERTY(int currentDut WRITE setCurrentDut READ getCurrentDut)
    Q_PROPERTY(int totalTested WRITE setTotalTested READ getTotalTested)
    Q_PROPERTY(int successCount WRITE setSuccessCount READ getSuccessCount)
    Q_PROPERTY(int failedCount WRITE setFailedCount READ getFailedCount)
    Q_PROPERTY(bool started WRITE setStarted READ isStarted)

public:

    explicit SessionManager(QObject *parent = nullptr);
    ~SessionManager();

    QList<Dut*>& getDutList()
    {
        return m_dutList;
    }

    Dut* getDut(int no) const {return m_dutList.at(no - 1);}

    QString getOperatorName() const
    {
        return m_operatorName;
    }

    int getCurrentDut() const
    {
        return m_currentDut;
    }

    bool isStarted() const
    {
        return m_started;
    }

    QString getStartTime() const
    {
        return m_startTime;
    }

    QString getBatchNumber() const
    {
        return m_batchNumber;
    }

    QString getBatchInfo() const
    {
        return m_batchInfo;
    }

    int getTotalTested() const
    {
        return m_totalTested;
    }

    int getSuccessCount() const
    {
        return m_successCount;
    }

    int getFailedCount() const
    {
        return m_failedCount;
    }

public slots:

    void resetDutList();
    void uncheckAllDuts();

    void setOperatorName(QString operatorName)
    {
        m_operatorName = operatorName;
    }

    void setCurrentDut(int currentDut)
    {
        m_currentDut = currentDut;
    }

    void setStarted(bool started)
    {
        m_started = started;
    }

    void setStartTime(QString startTime)
    {
        m_startTime = startTime;
    }

    void setBatchNumber(QString batchNumber)
    {
        m_batchNumber = batchNumber;
    }

    void setBatchInfo(QString batchInfo)
    {
        m_batchInfo = batchInfo;
    }

    void setTotalTested(int totalTested)
    {
        m_totalTested = totalTested;
    }

    void setSuccessCount(int successCount)
    {
        m_successCount = successCount;
    }

    void setFailedCount(int failedCount)
    {
        m_failedCount = failedCount;
    }

private:

    QString m_operatorName;
    int m_currentDut = 0;
    bool m_started = false;
    QString m_startTime;
    QString m_batchNumber;
    QString m_batchInfo;
    int m_totalTested = 0;
    int m_successCount = 0;
    int m_failedCount = 0;

    QList<Dut*> m_dutList;
};

#endif // SESSIONMANAGER_H
