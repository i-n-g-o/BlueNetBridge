#include "messagereceiver.h"


static QString packetTerminator = "\n\r";

MessageReceiver::MessageReceiver() :
    packet("")
  ,m_device(0)
{}

MessageReceiver::MessageReceiver(DeviceInfo *device) : MessageReceiver()
{
    m_device = device;
}


void MessageReceiver::messageReceived(QString value)
{
    QString current = "";
    packet += value;

    QStringList split = packet.split(packetTerminator);
    if (split.length() > 1) {

        current = split[0];
        split.pop_front();

        packet = split.join(packetTerminator);
    }

    if (current.length() > 0) {
        if (current.startsWith("http://", Qt::CaseInsensitive)) {
            qDebug() << "http request: " << current;
            startRequest(QUrl(current));

        } else {
    //        qDebug() << "other message: " << value;
        }
    }
}


void MessageReceiver::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;

    reply = qnam.get(QNetworkRequest(url));

    connect(reply,
            &QNetworkReply::finished,
            this,
            &MessageReceiver::httpFinished);

    connect(reply,
            &QIODevice::readyRead,
            this,
            &MessageReceiver::httpReadyRead);

    connect(reply,
            &QNetworkReply::downloadProgress,
            this,
            &MessageReceiver::downloadProgress);
}


void MessageReceiver::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        qDebug() << "progress: " << (bytesReceived / bytesTotal);
    }
}

void MessageReceiver::httpReadyRead()
{
    qDebug() << "ready read";
}


void MessageReceiver::httpFinished()
{

    if (reply->error()) {
        qDebug() << tr("http failed:\n%1.").arg(reply->errorString());

        reply->deleteLater();
        reply = nullptr;
        return;
    }

    // deal with redirection
    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (!redirectionTarget.isNull()) {

        reply->deleteLater();
        reply = nullptr;

        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());

        qDebug() << "redirect: " << redirectedUrl;

        startRequest(redirectedUrl);
        return;
    }

    // get data from reply

    if (m_device) {
        m_device->writeData(reply->readAll());
    }

    // done with reply
    reply->deleteLater();
    reply = nullptr;
}
