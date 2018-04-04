#ifndef DUMMYBLEDEVICE_H
#define DUMMYBLEDEVICE_H

#include <QBluetoothUuid>

#include "deviceinfo.h"

class DummyBLEDevice : public DeviceInfo
{
public:
    DummyBLEDevice();

    // overwrite functions
    virtual QString getAddress() const;
    virtual QString getName() const;

    // deviceInfo interface
    virtual void connectDevice();
    virtual void disconnectDevice();

public slots:
    virtual void connectResult(const bool& status);
    virtual void writeData(const QByteArray& data) {}

private:
    QString myAddress;
    QString myName;
};

#endif // DUMMYBLEDEVICE_H
