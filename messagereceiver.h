#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "deviceinfo.h"

class MessageReceiver : public QObject
{
    Q_OBJECT
public:
    MessageReceiver();
    MessageReceiver(DeviceInfo* device);

    DeviceInfo* getDevice() {
        return m_device;
    }
    void setDevice(DeviceInfo* device) {
        m_device = device;
    }

signals:

public slots:
    void messageReceived(QString value);

    // http request
    void httpReadyRead();
    void httpFinished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    void startRequest(const QUrl &requestedUrl);

    QString packet;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    DeviceInfo* m_device;
};

#endif // MESSAGERECEIVER_H
