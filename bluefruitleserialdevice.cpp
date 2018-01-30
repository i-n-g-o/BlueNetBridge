#include "bluefruitleserialdevice.h"

#include <QObject>
#include <QTimer>

const QBluetoothUuid BluefruitLESerialDevice::ServiceId = QBluetoothUuid(QString("6e400001-b5a3-f393-e0a9-e50e24dcca9e"));


BluefruitLESerialDevice::BluefruitLESerialDevice(const QBluetoothDeviceInfo &d) : DeviceInfo(d)
  ,controller(0)
  ,m_service(0)
  ,connecting(false)
  ,connected(false)
{
    // set device
    messageReceiver.setDevice(this);
}

BluefruitLESerialDevice::~BluefruitLESerialDevice()
{
   disconnectDevice();
}


void BluefruitLESerialDevice::connectDevice() {

    qDebug() << "BluefruitLESerialDevice::connect()";

    // connect to service...
    disconnectDevice();

    qDebug() << "creating controller";

    // Connecting signals and slots for connecting to LE services.
    controller = new QLowEnergyController(getDevice());

    connect(controller, &QLowEnergyController::connected,
            this, &BluefruitLESerialDevice::deviceConnected);
    connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error),
            this, &BluefruitLESerialDevice::errorReceived);
    connect(controller, &QLowEnergyController::disconnected,
            this, &BluefruitLESerialDevice::deviceDisconnected);

    connect(controller, &QLowEnergyController::stateChanged,
            this, &BluefruitLESerialDevice::deviceStateChanged);


    // service discovery
    connect(controller, &QLowEnergyController::serviceDiscovered,
            this, &DeviceInfo::lowEnergyServiceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &DeviceInfo::serviceScanDone);


//    if (isRandomAddress()) {
//        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
//    } else {
//    }
    controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();

    // start a watchdog
    // if device is blocked by other host, we would try to connect indefinetly

//    QTimer watchdog;
//    qDebug() << ".....";/*
//    QTimer::singleShot(10000, this, SLOT(BluefruitLESerialDevice::*/watchdogTimeout()));


    connecting = true;
    emit connectedChanged();
}

void BluefruitLESerialDevice::disconnectDevice() {


    if (controller) {
        qDebug() << "disconnecting from BLE Controler";

        controller->disconnectFromDevice();
        delete controller;

        controller = 0;
        m_service = 0;
    }

    connecting = false;
    connected = false;

    emit connectedChanged();
}


bool BluefruitLESerialDevice::isConnecting() const
{
    return connecting;
}

bool BluefruitLESerialDevice::isConnected() const
{
    return connected;
}

void watchdogTimeout()
{
    qDebug() << "TIMEROUT!!!!";
}




/*
 *
 * QLowEnergyController realted
 *
 *
*/


void BluefruitLESerialDevice::deviceConnected()
{
    qDebug() << "connected to device";

    controller->discoverServices();
}


void BluefruitLESerialDevice::errorReceived(QLowEnergyController::Error error)
{
    qDebug() << "ERROR: " << error;
}


void BluefruitLESerialDevice::deviceDisconnected() {

    qDebug() << "device disconnected! -> trying to reconnent - not implemented";

    delete controller;
    controller = 0;

    connecting = false;
    connected = false;
    emit connectedChanged();
}


void BluefruitLESerialDevice::deviceStateChanged(QLowEnergyController::ControllerState state)
{
    qDebug() << "deviceStateChanged: " << state;
}


/*
 *
 * lowEnergyServiceDiscovered
 *
*/
void BluefruitLESerialDevice::lowEnergyServiceDiscovered(const QBluetoothUuid &uuid)
{
    if (uuid != ServiceId) {
        qDebug() << "other service: " << uuid.toString();
        return;
    }

    m_service = controller->createServiceObject(uuid);

    if (!m_service) {
        qDebug() << "no service!";
        return;
    }

    if (m_service->state() == QLowEnergyService::DiscoveryRequired) {

        connect(m_service, &QLowEnergyService::stateChanged,
                this, &DeviceInfo::serviceDetailsDiscovered);

        // get characteristic updates
        connect(m_service,
                &QLowEnergyService::characteristicChanged,
                this,
                &DeviceInfo::mycharacteristicsUpdated);

        // get info about read/write
        connect(m_service, &QLowEnergyService::characteristicRead,
                this, &DeviceInfo::mycharacteristicRead);
        connect(m_service, &QLowEnergyService::characteristicWritten,
                this, &DeviceInfo::mycharacteristicWritten);

//        connect(service, &QLowEnergyService::descriptorRead,
//                this, &Device::descriptorRead);
//        connect(service, &QLowEnergyService::descriptorWritten,
//                this, &Device::descriptorWritten);


        // discover details
        m_service->discoverDetails();

    } else if (m_service->state() == QLowEnergyService::ServiceDiscovered) {
        // service details already done...
        qDebug() << "service details already done...";
    } else {
        qDebug() << "QLowEnergyService state: " << m_service->state();
    }
}


void BluefruitLESerialDevice::serviceScanDone()
{
    qDebug() << "BluefruitLESerialDevice::serviceScanDone";

    if (!m_service) {
        // no service???
        connecting = false;
        connected = false;
        emit connectedChanged();
    }
}


// QLowEnergyService related
void BluefruitLESerialDevice::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        // do not hang in "Scanning for characteristics" mode forever
        // in case the service discovery failed
        // We have to queue the signal up to give UI time to even enter
        // the above mode
        if (newState != QLowEnergyService::DiscoveringServices) {

            qDebug() << "-------";
//            QMetaObject::invokeMethod(this, "characteristicsUpdated",
//                                      Qt::QueuedConnection);
        }

        qDebug() << "serviceDetailsDiscovered state: " << newState;

        return;
    }


//    QLowEnergyService* service = qobject_cast<QLowEnergyService *>(sender());
    if (!m_service) {
        qDebug() << "no service...";
        return;
    }

    // analyse characteristics
    const QList<QLowEnergyCharacteristic> chars = m_service->characteristics();
    foreach (const QLowEnergyCharacteristic &ch, chars) {

//        qDebug() << "characteristic: " << ch.name() << \
//                    "\n\tuuid: " << ch.uuid().toString() << \
//                    "\n\tvalid: " << ch.isValid() << \
//                    "\n\tvalue: " << ch.value();

//        for (QLowEnergyDescriptor& d : ch.descriptors()) {
//            qDebug() << "d: " << d.name() << " : " << d.uuid() << " : " << d.value();
//        }

        QLowEnergyDescriptor descriptor = ch.descriptor(QBluetoothUuid(QString("00002902-0000-1000-8000-00805f9b34fb")));

        if (descriptor.isValid()) {
            // turn updates on
            m_service->writeDescriptor(descriptor, QByteArray::fromHex("0100"));
        }

        if (ch.properties() & QLowEnergyCharacteristic::Notify) {
        }
        else if (ch.properties() & QLowEnergyCharacteristic::Write) {

            if (writeCharacteristic.isValid()) {
                qDebug() << "writeCharacteristic already valid - overwriting!!";
            }

            writeCharacteristic = ch;
        }
    }

    if (writeCharacteristic.isValid()) {
        connecting = false;
        connected = true;
        emit connectedChanged();

        // store connection somwhere

    } else {
        qDebug("write characteristic not valid!! - abort");
        disconnectDevice();
    }
}



/*
 *
 * QLowEnergyCharacteristic realted
 *
 *
*/
void BluefruitLESerialDevice::mycharacteristicsUpdated(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    messageReceiver.messageReceived(QString(value));
}


void BluefruitLESerialDevice::mycharacteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    qDebug() << "mycharacteristicRead: " << value;
}

void BluefruitLESerialDevice::mycharacteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    qDebug() << "mycharacteristicWritten: " << value;
}


/*
 *
 * QLowEnergyCharacteristic realted
 *
 *
*/
void BluefruitLESerialDevice::writeData(const QByteArray& data)
{
    if (writeCharacteristic.isValid()) {
        m_service->writeCharacteristic(writeCharacteristic, data, QLowEnergyService::WriteWithoutResponse);
    } else {
        qDebug() << "characterstic not valid: " << writeCharacteristic.uuid();
    }
}
