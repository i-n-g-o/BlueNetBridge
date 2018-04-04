#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QList>
#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QSettings>

#include "bleserialdevice.h"

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
    void addDummyDevice();
    void connectDevice(QString deviceId);
    void disconnectDevice(QString deviceId);

    // settings
    void clearSettings();

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
    void setupBLEController();
    void setUpdate(QString message);
    void writeSettings();
    void readSettings();
    void executeDevice(const QString& deviceAddress, void (*func)(BLESerialDevice* di));
    bool containsDevice(const QString& deviceAddress) const;


    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller;

    QSettings settings;
    QStringList shouldConnectDevices;

    QList<QObject*> devices;
    bool scanning;

};

#endif // DEVICECONTROLLER_H
