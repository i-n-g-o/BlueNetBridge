#include "bluefruitleserialdevice.h"

#include <QObject>
#include <QTimer>

const QBluetoothUuid BluefruitLESerialDevice::ServiceId = QBluetoothUuid(QString("6e400001-b5a3-f393-e0a9-e50e24dcca9e"));

BluefruitLESerialDevice::BluefruitLESerialDevice(const QBluetoothDeviceInfo &d) : BLESerialDevice(d)
  ,controller(0)
  ,m_service(0)
{
    // set device
    connect(&messageReceiver,
            &MessageReceiver::data,
            this,
            &BluefruitLESerialDevice::writeData);
}

BluefruitLESerialDevice::~BluefruitLESerialDevice()
{
    qDebug() << "----------- ~BluefruitLESerialDevice";
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
            this, &BluefruitLESerialDevice::lowEnergyServiceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &BluefruitLESerialDevice::serviceScanDone);


//    if (isRandomAddress()) {
//        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
//    } else {
//    }
    controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();

    // start a watchdog
    // if device is blocked by other host, we would try to connect indefinetly
    QTimer::singleShot(10000, this, SLOT(watchdogTimeout()));


    setConnecting(true);
}

void BluefruitLESerialDevice::disconnectDevice() {


    if (controller) {
        qDebug() << "disconnecting from BLE Controler";

        controller->disconnectFromDevice();
        delete controller;

        controller = 0;
        m_service = 0;
    }

    setConnected(false);
}


void BluefruitLESerialDevice::testDevice()
{
    QFile file("/Users/inx/Desktop/bewerbblock.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    if (!file.isOpen()) {
        qDebug() << "file no open";
        return;
    }

    qDebug() << "testing";

//    qDebug() << "sending: " << data;

//    QByteArray data("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");
//    arr.append("1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31-32-33-34-35-36-37-38-39-40");

    bytessent = 0;
    startSending = QTime::currentTime();

    writeData(data);
}

void BluefruitLESerialDevice::watchdogTimeout()
{
    qDebug() << "TIMEROUT!!!!";
    disconnectDevice();
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

    setConnected(false);

    // if autoreconnect?
    connectDevice();
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

        connect(m_service,
                &QLowEnergyService::stateChanged,
                this,
                &BluefruitLESerialDevice::serviceDetailsDiscovered);

        // get characteristic updates
        connect(m_service,
                &QLowEnergyService::characteristicChanged,
                this,
                &BluefruitLESerialDevice::mycharacteristicsUpdated);

        // get info about read/write
        connect(m_service, &QLowEnergyService::characteristicRead,
                this, &BluefruitLESerialDevice::mycharacteristicRead);
        connect(m_service, &QLowEnergyService::characteristicWritten,
                this, &BluefruitLESerialDevice::mycharacteristicWritten);

//        connect(m_service, &QLowEnergyService::descriptorRead,
//                this, &BluefruitLESerialDevice::descriptorRead);
//        connect(m_service, &QLowEnergyService::descriptorWritten,
//                this, &BluefruitLESerialDevice::descriptorWritten);


        //void error(QLowEnergyService::ServiceError error);
//        connect(m_service, &QLowEnergyService::error,
//                this, &BluefruitLESerialDevice::serviceError);


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
        setConnected(false);
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

            //QLowEnergyCharacteristic::PropertyTypes props = writeCharacteristic.properties();
        }
    }

    if (writeCharacteristic.isValid()) {
        setConnected(true);
        writeData(QString("init").toUtf8());
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
    messageReceiver.dataReceived(value);
}


void BluefruitLESerialDevice::mycharacteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    qDebug() << "mycharacteristicRead: " << value;
}

void BluefruitLESerialDevice::mycharacteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    qDebug() << "mycharacteristicWritten: " << value;

    bytessent += value.size();

    if (value == dataSending) {
        // ok send next...
        if (dataToSend.isEmpty()) {
            // sending done!

            dataSending.clear();
            if (!value.endsWith(messageReceiver.getPacketTerminatorOut())) {
                m_service->writeCharacteristic(writeCharacteristic, messageReceiver.getPacketTerminatorOut(), QLowEnergyService::WriteWithResponse);
            }

        } else {
            writeData(dataToSend);
        }

    } else if (!dataSending.isEmpty()) {
        qDebug() << "send data does not match!";
    } else {

        qDebug() << "sent " << bytessent << " in " << startSending.restart() << " ms";

    }

}

void BluefruitLESerialDevice::serviceError(QLowEnergyService::ServiceError error)
{
    qDebug() << "ERROR: " << error;
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

        // get the first 20 bytes
        dataSending = data.left(20);
        dataToSend = data.right(data.size() - 20);

        if (dataSending.isEmpty()) {
            qDebug() << "nothing to send";
            return;
        }

        qDebug() << "writing: " << dataSending;
        m_service->writeCharacteristic(writeCharacteristic, dataSending, QLowEnergyService::WriteWithResponse); //WriteWithoutResponse

        auto err = m_service->error();
        if (err !=  QLowEnergyService::NoError) {
            qDebug() << "error: " << err;
        }

    } else {
        qDebug() << "characterstic not valid: " << writeCharacteristic.uuid();
    }
}
