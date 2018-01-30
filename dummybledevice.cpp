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

    QTimer timer;
    timer.singleShot(1000, this, SLOT(connectResult()));
    timer.start();
}

void DummyBLEDevice::disconnectDevice() {
    qDebug() << "DummyBLEDevice::disconnect()" << getName();
}


QString DummyBLEDevice::getAddress() const
{
    return myAddress;
}

QString DummyBLEDevice::getName() const
{
    return myName;
}


void DummyBLEDevice::connectResult()
{
    int randomValue = qrand() % 100;
    if (randomValue > 50) {
        qDebug() << "result true";
    } else {
        qDebug() << "result failed";
    }
}
