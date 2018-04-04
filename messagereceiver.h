#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "bleserialdevice.h"

class MessageReceiver : public QObject
{
    Q_OBJECT
public:
    static const QByteArray defaultPacketTerminatorIn;
    static const QByteArray defaultPacketTerminatorOut;

    MessageReceiver();

    void setPacketTerminatorIn(const QByteArray& pt) { packetTerminatorIn = pt; }
    const QByteArray& getPacketTerminatorIn() const { return packetTerminatorIn; }

    void setPacketTerminatorOut(const QByteArray& pt) { packetTerminatorOut = pt; }
    const QByteArray& getPacketTerminatorOut() const { return packetTerminatorOut; }


signals:
    void data(const QByteArray& data);

public slots:
    void dataReceived(const QByteArray& value);

    // http request
    void httpReadyRead();
    void httpFinished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    void startRequest(const QUrl &requestedUrl);
    void dataToDevice(const QByteArray& data);

    QByteArray packet;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    bool sendResponse;

    QByteArray packetTerminatorIn;
    QByteArray packetTerminatorOut;
};

#endif // MESSAGERECEIVER_H
