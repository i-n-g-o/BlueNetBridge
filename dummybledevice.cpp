#include "dummybledevice.h"

#include <QUuid>
#include <QTimer>

DummyBLEDevice::DummyBLEDevice()
{
    myAddress = QUuid::createUuid().toString();

    int randomValue = qrand() % 100;
    myName = "Dummy: " + QString::number(randomValue);
}



void DummyBLEDevice::connectDevice() {

    qDebug() << "DummyBLEDevice::connect()" << getName();

    bool result = (qrand() % 100) > 50;
    QTimer::singleShot(2000, this, [this, result]{ connectResult(result); });

    setConnecting(true);
}

void DummyBLEDevice::disconnectDevice() {
    qDebug() << "DummyBLEDevice::disconnect()" << getName();

    setConnected(false);
}


QString DummyBLEDevice::getAddress() const
{
    return myAddress;
}

QString DummyBLEDevice::getName() const
{
    return myName;
}


void DummyBLEDevice::connectResult(const bool& status)
{
    qDebug() << "DummyBLEDevice::connectResult: " << status;

    setConnected(status);
}
