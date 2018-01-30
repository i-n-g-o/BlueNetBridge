#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothaddress.h>
#include <QList>
#include <QLowEnergyController>


#include "deviceinfo.h"

class DeviceInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getName NOTIFY deviceChanged)
    Q_PROPERTY(QString deviceAddress READ getAddress NOTIFY deviceChanged)
    Q_PROPERTY(bool deviceTracking READ getTracking WRITE setTracking NOTIFY deviceChanged)
    Q_PROPERTY(bool connecting READ isConnecting NOTIFY connectedChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    DeviceInfo();
    DeviceInfo(const QBluetoothDeviceInfo &d);

    virtual QString getAddress() const;
    virtual QString getName() const;

    bool getTracking() const;
    void setTracking(bool _tracking);

    virtual bool isConnecting() const = 0;
    virtual bool isConnected() const = 0;

    QBluetoothDeviceInfo getDevice();
    //void setDevice(const QBluetoothDeviceInfo &dev);

    virtual void connectDevice() = 0;
    virtual void disconnectDevice() = 0;

public slots:
    virtual void connectResult() = 0;
    virtual void writeData(const QByteArray& data) = 0;

    // QLowEnergyController realted
    virtual void deviceConnected() = 0;
    virtual void errorReceived(QLowEnergyController::Error) = 0;
    virtual void deviceDisconnected() = 0;

//    virtual void deviceStateChanged(QLowEnergyController::ControllerState state) = 0;

    // service discovery
    virtual void lowEnergyServiceDiscovered(const QBluetoothUuid &uuid) = 0;
    virtual void serviceScanDone() = 0;

    // QLowEnergyService related
    virtual void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState) = 0;

    // characteristics related
    virtual void mycharacteristicsUpdated(const QLowEnergyCharacteristic &c, const QByteArray &value) = 0;
    virtual void mycharacteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value) = 0;
    virtual void mycharacteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value) = 0;


Q_SIGNALS:
    void deviceChanged();
    void connectedChanged();

private:
    QBluetoothDeviceInfo device;
    bool tracking;
};

#endif // DEVICEINFO_H
