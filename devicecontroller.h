#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QList>
#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>

#include "deviceinfo.h"

/* object handles:
 * - device lookup (callback for new BLE devices)
 * - stop device discover, when connected to device?
 * - fill in list with found devices
 * - select a device, track it's connected status (connect to a device)
 * - holds a list of BLE Serial devices (with defined interface)

*/
class DeviceController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(bool scanning READ isScanning NOTIFY stateChanged)

public:
    explicit DeviceController(QObject *parent = nullptr);
    ~DeviceController();
    QVariant getDevices();
    bool isScanning();

signals:

public slots:
    bool startDeviceDiscovery();
    void addSomething();
    void trackDevice(QString deviceId, bool doTrack);

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void deviceDiscovered(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanCanceled();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

Q_SIGNALS:
    void devicesUpdated();
    void stateChanged();

private:
    void setUpdate(QString message);

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller;

    QList<QObject*> devices;

    bool connected;
    bool scanning;

};

#endif // DEVICECONTROLLER_H
