#include "devicecontroller.h"

#include <QCoreApplication>

#include "bluefruitleserialdevice.h"
#include "bledummybledevice.h"

DeviceController::DeviceController(QObject *parent) : QObject(parent)
{
    scanning = false;

    setupBLEController();

    settings.setFallbacksEnabled(false);
    readSettings();    
    qDebug() << "settings: " << settings.fileName();
    qDebug() << "try to autoconnect: " << shouldConnectDevices;
}


DeviceController::~DeviceController()
{
    // disconnectFromDevice();
    delete discoveryAgent;

    //delete controller;
    qDeleteAll(devices);

    devices.clear();
}

void DeviceController::setupBLEController()
{
    // forever searching discovery...?
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &DeviceController::deviceDiscovered);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &DeviceController::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &DeviceController::deviceScanFinished);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &DeviceController::deviceScanCanceled);
}

void DeviceController::readSettings()
{
    shouldConnectDevices.clear();
    int size = settings.beginReadArray("devices");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        shouldConnectDevices.append(settings.value("device").toString());
    }
    settings.endArray();
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


void DeviceController::addDummyDevice()
{
    BLEDummyDevice *d = new BLEDummyDevice();
    devices.append(d);

    emit devicesUpdated();
}

void DeviceController::connectDevice(QString deviceId)
{
    executeDevice(deviceId, [](BLESerialDevice* di){ di->connectDevice(); });

    if (!shouldConnectDevices.contains(deviceId)) {
        shouldConnectDevices.append(deviceId);
        writeSettings();
    }
}

void DeviceController::disconnectDevice(QString deviceId)
{
    executeDevice(deviceId, [](BLESerialDevice* di){ di->disconnectDevice(); });

    if (shouldConnectDevices.contains(deviceId)) {
        shouldConnectDevices.removeAll(deviceId);
        writeSettings();
    }
}

void DeviceController::writeSettings()
{
    settings.beginWriteArray("devices");
    for (int i = 0; i < shouldConnectDevices.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("device", shouldConnectDevices[i]);
    }
    settings.endArray();

    settings.sync();
}

void DeviceController::clearSettings()
{
    qDebug() << "clearing settings";
    shouldConnectDevices.clear();
    settings.clear();
    settings.sync();
}

/**
 * @brief DeviceController::executeDevice
 *      lookup a device with an address and execute call function
 *
 * @param deviceAddress
 */
void DeviceController::executeDevice(const QString& deviceAddress, void (*func)(BLESerialDevice* di))
{
    if (!func) {
        return;
    }

    for (QObject* obj : devices) {
        BLESerialDevice* di = dynamic_cast<BLESerialDevice*>(obj);
        if (di->getAddress() == deviceAddress) {
            return func(di);
        }
    }

    qDebug() << "could not find device with id: " << deviceAddress;
}

bool DeviceController::containsDevice(const QString& deviceAddress) const
{
    for (QObject* obj : devices) {
        BLESerialDevice* di = dynamic_cast<BLESerialDevice*>(obj);
        if (di->getAddress() == deviceAddress) {
            return true;
        }
    }

    return false;
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
        qDebug() << "already scanning";
        return false;
    }

    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

    if (discoveryAgent == nullptr) {
        setupBLEController();
    }

    setUpdate("Scanning for devices ...");

    // start device discovery
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);


    if (discoveryAgent->isActive()) {
        scanning = true;
        Q_EMIT stateChanged();
        return discoveryAgent->isActive();
    }

    qDebug() << "discover agent did not start";
    delete discoveryAgent;
    discoveryAgent = nullptr;
    return false;
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

                if (!containsDevice(BLESerialDevice::getAddress(info))) {

                    BLESerialDevice *d = new BluefruitLESerialDevice(info);
                    devices.append(d);
                    emit devicesUpdated();

                    qDebug() << "Last device added: " << d->getName() <<
                                " : " << d->getAddress() <<
                                " : " << d->getDevice().rssi() <<
                                " : " << d->getDevice().serviceUuids();
                }


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
    qDebug() << "deviceScanFinished!!";

    scanning = false;
    emit devicesUpdated();
    emit stateChanged();

    // check settings, if we should connect to something
    if (devices.size() > 0 && shouldConnectDevices.size() > 0) {

        for (QString& deviceId : shouldConnectDevices) {
            qDebug() << "trying to autoconnect to: " << deviceId;
            executeDevice(deviceId, [](BLESerialDevice* di) { di->connectDevice(); });
        }
    }
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
    }
}
