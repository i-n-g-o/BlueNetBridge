#ifndef BLUEFRUITLESERIALDEVICE_H
#define BLUEFRUITLESERIALDEVICE_H

#include <QBluetoothUuid>
#include <QLowEnergyController>

#include "bleserialdevice.h"
#include "messagereceiver.h"


/*
 * Adafruit Bluefriend
 *  nRF51822
 *
 */

class BluefruitLESerialDevice : public BLESerialDevice
{

public:
    static const QBluetoothUuid ServiceId;

    BluefruitLESerialDevice(const QBluetoothDeviceInfo &d);
    ~BluefruitLESerialDevice();

    // deviceInfo interface
    virtual void connectDevice();
    virtual void disconnectDevice();        


public slots:
    virtual void connectResult(const bool& status) {
        qDebug() << "BluefruitLESerialDevice::connectResult: " << status;
    }
    virtual void writeData(const QByteArray& data);
    virtual void testDevice();

private slots:
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
    void serviceError(QLowEnergyService::ServiceError error);

    void watchdogTimeout();

private:    
    QLowEnergyController* controller;
    QLowEnergyService* m_service;
    QLowEnergyCharacteristic writeCharacteristic;

    QByteArray dataSending;
    QByteArray dataToSend;

    QTime startSending;
    int bytessent;

    MessageReceiver messageReceiver;

};

#endif // BLUEFRUITLESERIALDEVICE_H
