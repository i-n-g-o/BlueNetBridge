#include "appcontroller.h"

#include "qdebug.h"

appController::appController(QObject *parent) : QObject(parent)
{

}

void appController::appStateChanged(Qt::ApplicationState state)
{
    //qDebug() << "----------------------------------->>>>>  app state changed: " << state;
}
