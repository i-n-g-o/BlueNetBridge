#include "bledummybledevice.h"

#include <QUuid>
#include <QTimer>

BLEDummyDevice::BLEDummyDevice()
{
    myAddress = QUuid::createUuid().toString();

    int randomValue = qrand() % 100;
    myName = "Dummy: " + QString::number(randomValue);
}



void BLEDummyDevice::connectDevice() {

    qDebug() << "DummyBLEDevice::connect()" << getName();

    bool result = (qrand() % 100) > 50;
    QTimer::singleShot(2000, this, [this, result]{ connectResult(result); });

    setConnecting(true);
}

void BLEDummyDevice::disconnectDevice() {
    qDebug() << "DummyBLEDevice::disconnect()" << getName();

    setConnected(false);
}


QString BLEDummyDevice::getAddress() const
{
    return myAddress;
}

QString BLEDummyDevice::getName() const
{
    return myName;
}


void BLEDummyDevice::connectResult(const bool& status)
{
    qDebug() << "DummyBLEDevice::connectResult: " << status;

    setConnected(status);
}
