#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "devicecontroller.h"
#include "appcontroller.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //
//    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setOrganizationName("i-n-g-o");
    QCoreApplication::setApplicationName("BLEBridge");

    QSettings::setDefaultFormat(QSettings::NativeFormat);

    QGuiApplication app(argc, argv);

    appController appCtrl;
    QObject::connect(&app,
                     SIGNAL(applicationStateChanged(Qt::ApplicationState)),
                     &appCtrl,
                     SLOT(appStateChanged(Qt::ApplicationState)));

    QQmlApplicationEngine engine;
    DeviceController deviceController;

    engine.rootContext()->setContextProperty("device", dynamic_cast<QObject*>(&deviceController));
    //engine.rootContext()->setContextProperty("device", QVariant::fromValue(deviceController)); //??
    engine.rootContext()->setContextProperty("devicelist", deviceController.getDevices());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qDebug() << "QML error: no root objects";
        return -1;
    }

    // this a setting??
    // start discovery
    deviceController.startDeviceDiscovery();

    return app.exec();
}
