#ifndef DBUSCONNECTION_H
#define DBUSCONNECTION_H

#include <QObject>
#include <QDebug>
#include "qtimer.h"
#include <qthread.h>

//#define SIMULATION

class DbusConnection : public QThread
{
    Q_OBJECT
public:
    explicit DbusConnection(QObject *parent = nullptr);
    ~DbusConnection();
    virtual void run() override;
    bool StartCharging(QString portId, QString voltage);
    bool StopCharging(QString portId, QString voltage);
    void ReadPortData();
    void StopPortData();
    void addReadPort(QString portId);
    void removeReadPort();
    void handle_msg_from_SMART_CHARGER_APP_module();
    bool dbus_emitter();
    void parseReceivedData(QString inputData);
    bool StartGrid();
    bool StopGrid();
    void StatusSignalSlot();
    bool isChargingStarted = false;

signals:
    void DBusDataReceived(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);

private:
    bool dbus_listen = true;
    bool threadRunning = false;
    QString selectedPort = "";

};

#endif // DBUSCONNECTION_H
