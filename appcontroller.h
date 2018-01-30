#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class appController : public QObject
{
    Q_OBJECT
public:
    explicit appController(QObject *parent = nullptr);

signals:

public slots:
    void appStateChanged(Qt::ApplicationState state);
};

#endif // APPCONTROLLER_H
