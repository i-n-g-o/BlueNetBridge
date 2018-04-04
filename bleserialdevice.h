#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothaddress.h>
#include <QList>
#include <QLowEnergyController>
#include <QTimer>

#include "deviceinfo.h"

class DeviceInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString deviceName READ getName NOTIFY deviceChanged)
    Q_PROPERTY(QString deviceAddress READ getAddress NOTIFY deviceChanged)
    Q_PROPERTY(bool connecting READ isConnecting NOTIFY connectedChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    static QString getAddress(const QBluetoothDeviceInfo &d);

    DeviceInfo();
    DeviceInfo(const QBluetoothDeviceInfo &d);

    virtual QString getAddress() const;
    virtual QString getName() const;

    virtual bool isConnecting() const {
        return connecting;
    }

    virtual bool isConnected() const {
        return connected;
    }

    QBluetoothDeviceInfo getDevice();
    //void setDevice(const QBluetoothDeviceInfo &dev);

    virtual void connectDevice() = 0;
    virtual void disconnectDevice() = 0;

protected:
    void setConnecting(const bool& status) {
        connecting = status;
        emit connectedChanged();
    }

    void setConnected(const bool& status) {
        connecting = false;
        connected = status;
        emit connectedChanged();
    }



public slots:
    virtual void connectResult(const bool& status) = 0;
    virtual void writeData(const QByteArray& data) = 0;


Q_SIGNALS:
    void deviceChanged();
    void connectedChanged();

private:
    QBluetoothDeviceInfo device;

    bool connecting;
    bool connected;
};

#endif // DEVICEINFO_H
