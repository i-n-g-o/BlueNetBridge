#include <QOperatingSystemVersion>

#include "messagereceiver.h"

const QByteArray MessageReceiver::defaultPacketTerminatorIn = "\n\r";
const QByteArray MessageReceiver::defaultPacketTerminatorOut = "\\n\\r";

static const int defaultPacketTerminatorInLength = MessageReceiver::defaultPacketTerminatorIn.size();

MessageReceiver::MessageReceiver() :
    packet("")
  ,reply(nullptr)
  ,sendResponse(true)
  ,packetTerminatorIn(defaultPacketTerminatorIn)
  ,packetTerminatorOut(defaultPacketTerminatorOut)
{}



void MessageReceiver::dataReceived(const QByteArray& value)
{
    QString current;
    packet += value;

    int idx = packet.indexOf(packetTerminatorIn);
    if (idx >= 0) {
        current = QString(packet.left(idx));
        packet = packet.remove(0, idx + defaultPacketTerminatorInLength);
    }

    if (!current.isEmpty()) {
        if (current.startsWith("http://", Qt::CaseInsensitive) ||
            current.startsWith("https://", Qt::CaseInsensitive))
        {
            // start a request
            startRequest(QUrl(current));

        } else if (current.startsWith("c:", Qt::CaseInsensitive)) {

            // config - configure the bridge
            // response: 0|1 if the bridge should send a resonse or not


            QString config_command = current.toLower().replace("c:", "");

            if (config_command.startsWith("response:")) {

                if (current.replace("response:", "").toUInt() > 0) {
                    sendResponse = true;
                } else {
                    sendResponse = false;
                }

                qDebug() << "set sendResponse: " << sendResponse;

            } else if (config_command.startsWith("response")) {
                dataToDevice(QString::asprintf("c:response:%d", sendResponse).toUtf8());
            }

        } else if (current.startsWith("d:", Qt::CaseInsensitive)) {

            // device commands
            // info: some system information


            QString host_command = current.toLower().replace("d:", "");

            if (host_command.startsWith("info", Qt::CaseInsensitive)) {
                dataToDevice(QOperatingSystemVersion::current().name().toUtf8());
            }

        } else {
            //qDebug() << "other message: " << current;
        }
    }
}


void MessageReceiver::startRequest(const QUrl &requestedUrl)
{
    if (reply != nullptr) {

        if (sendResponse) {
            qDebug() << "waiting for response! dropping: " << requestedUrl;
            return;
        } else {
            reply->abort();
            reply->deleteLater();
            reply = nullptr;
        }
    }

    url = requestedUrl;

    reply = qnam.get(QNetworkRequest(url));

    connect(reply,
            &QNetworkReply::finished,
            this,
            &MessageReceiver::httpFinished);

    connect(reply,
            &QNetworkReply::readyRead,
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

    if (sendResponse) {
        // get data from reply
        QByteArray response_data = reply->readAll();
        dataToDevice(response_data.trimmed());
    }

    // done with reply
    reply->deleteLater();
    reply = nullptr;
}


void MessageReceiver::dataToDevice(const QByteArray& dataToSend)
{
    if (dataToSend.endsWith(packetTerminatorOut)) {
        emit data(dataToSend);
    } else {
        QByteArray d = dataToSend + packetTerminatorOut;
        emit data(d);
    }
}
