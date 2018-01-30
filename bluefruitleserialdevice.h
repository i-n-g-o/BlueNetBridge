#ifndef BLUEFRUITLESERIALDEVICE_H
#define BLUEFRUITLESERIALDEVICE_H

#include <QBluetoothUuid>
#include <QLowEnergyController>

#include "deviceinfo.h"
#include "messagereceiver.h"


/*
 * Adafruit Bluefriend
 *  nRF51822
 *
 */

class BluefruitLESerialDevice : public DeviceInfo
{

public:
    static const QBluetoothUuid ServiceId;

    BluefruitLESerialDevice(const QBluetoothDeviceInfo &d);
    ~BluefruitLESerialDevice();

    // deviceInfo interface
    virtual void connectDevice();
    virtual void disconnectDevice();

    virtual bool isConnecting() const;
    virtual bool isConnected() const;


public slots:
    virtual void connectResult() {}
    virtual void writeData(const QByteArray& data);

    // QLowEnergyController realted
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void deviceDisconnected();

    void deviceStateChanged(QLowEnergyController::ControllerState state);

    // service discovery
    void lowEnergyServiceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);

    // characteristics related
    void mycharacteristicsUpdated(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void mycharacteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value);
    void mycharacteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value);


public slots:
    void watchdogTimeout();

private:
    QLowEnergyController* controller;
    QLowEnergyService* m_service;
    QLowEnergyCharacteristic writeCharacteristic;

    MessageReceiver messageReceiver;

    bool connecting;
    bool connected;

};

#endif // BLUEFRUITLESERIALDEVICE_H