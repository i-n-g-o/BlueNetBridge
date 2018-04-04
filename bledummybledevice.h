#ifndef DUMMYBLEDEVICE_H
#define DUMMYBLEDEVICE_H

#include <QBluetoothUuid>

#include "bleserialdevice.h"

class BLEDummyDevice : public BLESerialDevice
{
public:
    BLEDummyDevice();
    ~BLEDummyDevice() {
        qDebug() << "----------- ~DummyBLEDevice";
    }

    // overwrite functions
    virtual QString getAddress() const;
    virtual QString getName() const;

    // deviceInfo interface
    virtual void connectDevice();
    virtual void disconnectDevice();    

public slots:
    virtual void connectResult(const bool& status);
    virtual void writeData(const QByteArray& data) {}
    virtual void testDevice() {
        qDebug() << "TEST DuMMY";
    }

private:
    QString myAddress;
    QString myName;
};

#endif // DUMMYBLEDEVICE_H
