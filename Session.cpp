#include "Session.h"

Session::Session(QObject *parent) : QObject(parent)
{

}

QString Session::getOperatorName() const {return operatorName;}

void Session::setOperatorName(const QString &name) {operatorName = name;}

int Session::getCurrentDut() const {return currentDut;}

void Session::setCurrentDut(int index) {currentDut = index;}

QList<Dut> &Session::getDutsList() {return duts;}
