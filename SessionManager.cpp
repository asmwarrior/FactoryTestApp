#include "SessionManager.h"

SessionManager::SessionManager(QObject *parent) : QObject(parent)
{
    resetDutList();
}

SessionManager::~SessionManager()
{

}

void SessionManager::resetDutList()
{
    for(auto & dut : m_dutList)
    {
        delete dut;
    }

    m_dutList.clear();

    m_dutList.push_back(new Dut(1, 1, 1));
    m_dutList.push_back(new Dut(2, 1, 2));
    m_dutList.push_back(new Dut(3, 1, 3));

    m_dutList.push_back(new Dut(4, 2, 1));
    m_dutList.push_back(new Dut(5, 2, 2));
    m_dutList.push_back(new Dut(6, 2, 3));

    m_dutList.push_back(new Dut(7, 3, 1));
    m_dutList.push_back(new Dut(8, 3, 2));
    m_dutList.push_back(new Dut(9, 3, 3));

    m_dutList.push_back(new Dut(10, 4, 1));
    m_dutList.push_back(new Dut(11, 4, 2));
    m_dutList.push_back(new Dut(12, 4, 3));

    m_dutList.push_back(new Dut(13, 5, 1));
    m_dutList.push_back(new Dut(14, 5, 2));
    m_dutList.push_back(new Dut(15, 5, 3));
}
