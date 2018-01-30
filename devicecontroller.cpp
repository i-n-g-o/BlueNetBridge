#include "devicecontroller.h"

#include <QCoreApplication>

#include "bluefruitleserialdevice.h"
#include "dummybledevice.h"

DeviceController::DeviceController(QObject *parent) : QObject(parent)
{
    scanning = false;

    // forever searching discovery...?
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(10000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &DeviceController::deviceDiscovered);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &DeviceController::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &DeviceController::deviceScanFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &DeviceController::deviceScanCanceled);
}


DeviceController::~DeviceController()
{
    // disconnectFromDevice();
    delete discoveryAgent;

    //delete controller;
    qDeleteAll(devices);

    devices.clear();
}

QVariant DeviceController::getDevices()
{
    return QVariant::fromValue(devices);
}

bool DeviceController::isScanning()
{
    return scanning;
}

void DeviceController::setUpdate(QString message)
{
    qDebug() << message;
//    m_message = message;
//    emit updateChanged();
}


void DeviceController::addSomething()
{    
    DeviceInfo *d = new DummyBLEDevice();
    devices.append(d);

    emit devicesUpdated();
}

void DeviceController::trackDevice(QString deviceId, bool doTrack)
{
    qDebug() << "track: " << deviceId << " : " << doTrack;

    // get device from list
    for (QObject* obj : devices) {
        DeviceInfo* di = dynamic_cast<DeviceInfo*>(obj);
        if (di->getAddress() == deviceId) {
            di->setTracking(doTrack);
            break;
        }
    }
}


/*
 * http://www.patorjk.com/software/taag/#p=display&f=Doom&t=Device%20Discovery%20Agent
 *
 *   ______           _           ______ _                                      ___                   _
 *   |  _  \         (_)          |  _  (_)                                    / _ \                 | |
 *   | | | |_____   ___  ___ ___  | | | |_ ___  ___ _____   _____ _ __ _   _  / /_\ \ __ _  ___ _ __ | |_
 *   | | | / _ \ \ / / |/ __/ _ \ | | | | / __|/ __/ _ \ \ / / _ \ '__| | | | |  _  |/ _` |/ _ \ '_ \| __|
 *   | |/ /  __/\ V /| | (_|  __/ | |/ /| \__ \ (_| (_) \ V /  __/ |  | |_| | | | | | (_| |  __/ | | | |_
 *   |___/ \___| \_/ |_|\___\___| |___/ |_|___/\___\___/ \_/ \___|_|   \__, | \_| |_/\__, |\___|_| |_|\__|
 *                                                                      __/ |         __/ |
 *                                                                     |___/         |___/
*/

bool DeviceController::startDeviceDiscovery()
{
    if (scanning) {
        return false;
    }

    qDeleteAll(devices);
    devices.clear();

//    const QBluetoothDeviceInfo info(QBluetoothAddress(QString("123")), "TEST TEST", 1);
//    DeviceInfo *d = new DeviceInfo(info);
//    devices.append(d);

    emit devicesUpdated();

    setUpdate("Scanning for devices ...");

    // start device discovery
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);


    if (discoveryAgent->isActive()) {
        scanning = true;
        Q_EMIT stateChanged();
    } else {
        qDebug() << "discover agent did not start";
    }

    return discoveryAgent->isActive();
}

// callbacks

void DeviceController::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {

        // TODO: check against a list with services...
        // for now hardcoded for Adafruit BLE Friend

        if (!info.serviceUuids().empty()) {
            // only accept devices which have service with uuid: 6e400001-b5a3-f393-e0a9-e50e24dcca9e
            if (info.serviceUuids().contains(BluefruitLESerialDevice::ServiceId)) {

                DeviceInfo *d = new BluefruitLESerialDevice(info);
                devices.append(d);

                emit devicesUpdated();

                qDebug() << "Last device added: " << d->getName() <<
                            " : " << d->getAddress() <<
                            " : " << d->getDevice().rssi() <<
                            " : " << d->getDevice().serviceUuids();
            } else {
                qDebug() << "other device with services: " << info.serviceUuids();
            }
        } else {
            qDebug() << "no services: " << info.name();
        }
    }
}


void DeviceController::deviceScanFinished()
{
    // should never happen??
    qDebug() << "deviceScanFinished!!";

    scanning = false;
    emit devicesUpdated();
    emit stateChanged();
//    m_deviceScanState = false;
//
//    if (devices.isEmpty()) {
//        setUpdate("No matching Low Energy devices found...");
//        QCoreApplication::quit();
//    } else {
//        setUpdate("Done! Scan Again!");

//        // start service discovery
//        scanServices(currentDevice.getDevice().address().toString());
//    }
}

void DeviceController::deviceScanCanceled()
{
    qDebug() << "deviceScanCanceled!!";

    scanning = false;
    emit devicesUpdated();
    emit stateChanged();
}


void DeviceController::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError){
        setUpdate("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    }else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError) {
        setUpdate("Writing or reading from the device resulted in an error.");
    } else if (error == QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError) {
        setUpdate("Invalid BLE adapter...");
        discoveryAgent->stop();
    } else  {
        qDebug() << discoveryAgent->errorString();
        setUpdate("An unknown error has occurred.");
        QCoreApplication::quit();
    }
}
