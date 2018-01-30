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

    virtual bool isConnecting() const {}
    virtual bool isConnected() const {}

public slots:
    virtual void connectResult();
    virtual void writeData(const QByteArray& data) {}

    // QLowEnergyController realted
    void deviceConnected() {}
    void errorReceived(QLowEnergyController::Error) {}
    void deviceDisconnected() {}

//    virtual void deviceStateChanged(QLowEnergyController::ControllerState state) {}

    void lowEnergyServiceDiscovered(const QBluetoothUuid &uuid) {}
    void serviceScanDone() {}

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState) {}

    // characteristics related
    virtual void mycharacteristicsUpdated(const QLowEnergyCharacteristic &c, const QByteArray &value) {}
    void mycharacteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value) {};
    void mycharacteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value) {};

private:
    QString myAddress;
    QString myName;
};

#endif // DUMMYBLEDEVICE_H
