#pragma once

//#include <QObject>
//#include <QStringList>
//#include <QDebug>

#include <QVariant>

enum DutState {inactive, untested, tested, warning};

using Dut = QMap<QString, QVariant>;

static Dut dutTemplate {
    {"state", DutState::inactive},
    {"id", ""},
    {"no", 0},
    {"checked", false},
    {"voltageChecked", false},
    {"accelChecked", false},
    {"lightSensChecked", false},
    {"daliChecked", false}
};

//class Dut : public QObject
//{
//    Q_OBJECT

//public:

//    enum DutState {inactive, untested, tested, warning};
//    Q_ENUM(DutState)

//    Q_PROPERTY(DutState state WRITE setState READ getState)
//    Q_PROPERTY(QString id WRITE setId READ getId)
//    Q_PROPERTY(int no WRITE setNo READ getNo)
//    Q_PROPERTY(int testPanel WRITE setTestPanel READ getTestPanel)
//    Q_PROPERTY(int pos WRITE setPos READ getPos)
//    Q_PROPERTY(bool checked WRITE setChecked READ isChecked)
//    Q_PROPERTY(QStringList errorList READ getErrorList)

//    Dut(int no, int testPanel, int pos) {m_no = no; m_testPanel = testPanel; m_pos = pos;}

//    DutState getState() const
//    {
//        return m_state;
//    }

//    QString getId() const
//    {
//        return m_id;
//    }

//    int getNo() const
//    {
//        return m_no;
//    }

//    int getTestPanel() const
//    {
//        return m_testPanel;
//    }

//    int getPos() const
//    {
//        return m_pos;
//    }

//    bool isChecked() const
//    {
//        return m_checked;
//    }

//    QStringList getErrorList() const
//    {
//        return m_errorList;
//    }

//signals:

//    void changed();

//public slots:
//    void setState(DutState state)
//    {
//        m_state = state;
//        emit changed();
//    }

//    void setId(QString id)
//    {
//        m_id = id;
//        emit changed();
//    }

//    void setNo(int no)
//    {
//        m_no = no;
//        emit changed();
//    }

//    void setTestPanel(int testPanel)
//    {
//        m_testPanel = testPanel;
//        emit changed();
//    }

//    void setPos(int pos)
//    {
//        m_pos = pos;
//        emit changed();
//    }

//    void setChecked(bool checked)
//    {
//        m_checked = checked;
//        emit changed();
//    }

//private:

//    DutState m_state = DutState::inactive;
//    QString m_id;
//    int m_no;
//    int m_testPanel;
//    int m_pos;
//    bool m_checked = false;
//    QStringList m_errorList;
//};
