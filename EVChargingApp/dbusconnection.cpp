#include "dbusconnection.h"
#include <QDebug>
#include "inc/dbus_listener.h"
#include "inc/dbus_emitter.h"

struct json_obj_t jdata;


DbusConnection::DbusConnection(QObject *parent)
    : QThread(parent)
{
}

DbusConnection::~DbusConnection()
{
    StopPortData();
}

void DbusConnection::ReadPortData()
{
    dbus_listen = true;
    this->start();
    qDebug()<<__func__;
}

void DbusConnection::StopPortData()
{

    // Waiting for the created thread to terminate
    qDebug()<<"StopPortData"<<threadRunning;
    dbus_listen = false;
    while(threadRunning)
    {
        QThread::msleep(100);
    }
    qDebug()<<"StopPortData"<<threadRunning;
}

void DbusConnection::run()
{
    dbus_listen = true;

#ifdef SIMULATION
    QThread::sleep(5);
    QString jsonResponse = R"({"status": {"port_id": 4, "action": "reset","voltage": 13.19,"current": 8.18, "percentage": 20.18}})";
    parseReceivedData(jsonResponse);
    QThread::sleep(5);
    jsonResponse = R"({"request": {"port_id": 1, "action": "set","voltage": 100}})";
    parseReceivedData(jsonResponse);
    QThread::sleep(5);
    jsonResponse = R"({"status": {"port_id": 1, "action": "set","voltage": 13.19,"current": 8.18, "percentage": 40.18}})";
    parseReceivedData(jsonResponse);
    QThread::sleep(5);
    jsonResponse = R"({"status": {"port_id": 1, "action": "reset","voltage": 13.19,"current": 50.18,"percentage": 90.18}})";
    parseReceivedData(jsonResponse);
#else
    StatusSignalSlot();
#endif
}

bool DbusConnection::StartGrid()
{
    signal_data.signal_id = LCD_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
    signal_data.cmd = REQUEST_SIGNAL;
    signal_data.payload= (char *)malloc(sizeof(char)*DATA_SIZE);
    QString data = GRID_ON;
    memcpy(signal_data.payload, data.toStdString().c_str(), data.size());
    signal_data.dlc = strlen(signal_data.payload);
    bool status = dbus_emitter();
    if(!status)
    {
        qDebug()<<__func__<<"dbus_emitter failed";
        return false;
    }

    delete signal_data.payload;

    return true;
}

bool DbusConnection::StopGrid()
{
    signal_data.signal_id = LCD_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
    signal_data.cmd = REQUEST_SIGNAL;
    signal_data.payload= (char *)malloc(sizeof(char)*DATA_SIZE);
    QString data = GRID_OFF;
    memcpy(signal_data.payload, data.toStdString().c_str(), data.size());
    signal_data.dlc = strlen(signal_data.payload);
    bool status = dbus_emitter();
    if(!status)
    {
        qDebug()<<__func__<<"dbus_emitter failed";
        return false;
    }

    delete signal_data.payload;

    return true;
}

void DbusConnection::addReadPort(QString portId)
{
    selectedPort = portId;
}
void DbusConnection::removeReadPort()
{
    selectedPort = "";
}

bool DbusConnection::StartCharging(QString portId, QString voltage)
{
    signal_data.signal_id = LCD_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
    signal_data.cmd = REQUEST_SIGNAL;
    signal_data.payload= (char *)malloc(sizeof(char)*DATA_SIZE);
    QString data;

    switch(portId.toInt())
    {
    case 1:
        data = PORT_A_ON;
        break;
    case 2:
        data = PORT_B_ON;
        break;
    case 3:
        data = PORT_C_ON;
        break;

    case 5:
        data = PORT_D_ON;
        break;
    default:
        qDebug()<<__func__<<"Invalid";
    }

    data = data.replace("VOLTAGECHANGE",voltage);
    memcpy(signal_data.payload, data.toStdString().c_str(), data.size());
    signal_data.dlc = strlen(signal_data.payload);
    bool status = dbus_emitter();
    if(!status)
    {
        qDebug()<<__func__<<"dbus_emitter failed";
        isChargingStarted = false;
        return false;
    }

    delete signal_data.payload;

    isChargingStarted = true;
    return true;
}

bool DbusConnection::StopCharging(QString portId, QString voltage)
{
    signal_data.signal_id = LCD_SER_SIGNAL_ID;
    signal_data.can_id = CAN_ID;
    signal_data.cmd = REQUEST_SIGNAL;
    signal_data.payload= (char *)malloc(sizeof(char)*DATA_SIZE);
    QString data;

    switch(portId.toInt())
    {
    case 1:
        data = PORT_A_OFF;
        break;
    case 2:
        data = PORT_B_OFF;
        break;
    case 3:
        data = PORT_C_OFF;
        break;
    case 5:
        data = PORT_D_OFF;
        break;
    default:
        qDebug()<<__func__<<"Invalid";
    }

    data = data.replace("VOLTAGECHANGE",voltage);
    memcpy(signal_data.payload, data.toStdString().c_str(), data.size());
    signal_data.dlc = strlen(signal_data.payload);
    bool status = dbus_emitter();
    if(!status)
    {
        qDebug()<<__func__<<" stop dbus_emitter failed";
        isChargingStarted = false;
        return false;
    }

    delete signal_data.payload;

    isChargingStarted = false;
    return true;
}

void DbusConnection::StatusSignalSlot()
{
    struct dbus_var_t dbus_info;

    // local json objected to extract data from dbus objects
    struct json_object *sig_id;

    char signal_id_str[DATA_SIZE];
    char get_json_data[DATA_SIZE];
    uint32_t signal_id;
    //init dbus and add filters
    read_init(&dbus_info);
    add_filter(&dbus_info, SMART_CHARGER_APP_SER_DATA_RCVR);

    threadRunning = true;

    while(dbus_listen)
    {
        if(check_data(&dbus_info) == 0)
        {
            jdata = read_data(&dbus_info);

            json_object_object_get_ex(jdata.obj, "sig_id", &jdata.sig);
            json_object_object_get_ex(jdata.obj, "dlc", &jdata.dlc);
            json_object_object_get_ex(jdata.obj, "can_id", &jdata.can_id);
            json_object_object_get_ex(jdata.obj, "cmd", &jdata.cmd);
            json_object_object_get_ex(jdata.obj, "payload", &jdata.payload);

            // get signal_id
            if(jdata.sig != NULL)
            {
                sprintf(signal_id_str, "%s", json_object_get_string(jdata.sig));
                signal_id = (int)strtol(signal_id_str, NULL, 16);
                qDebug()<< "signal id :"<< signal_id;
            }


            switch(signal_id)
            {
            case SMART_CHARGER_APP_SER_SIGNAL_ID:
                qDebug()<< "Msg from SMART CHARGER APP Module";
                handle_msg_from_SMART_CHARGER_APP_module();
                break;
            default:
                qDebug()<<"In the default switch case";
                break;
            }
        }
        QThread::msleep(250);
    }
    threadRunning = false;
}

void DbusConnection::handle_msg_from_SMART_CHARGER_APP_module()
{
    char get_json_dbus_payload[DATA_SIZE] = {0};
    uint32_t sig_cmd = 0;

    // get sig_cmd
    if(jdata.cmd == NULL)
    {
        qDebug()<<__func__<< "No signal command";
        return;
    }

    if(jdata.payload == NULL)
    {
        qDebug()<<__func__<< "No signal payload";
        return;
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.cmd));
    sig_cmd = (int)strtol(get_json_dbus_payload, NULL, 16);
    qDebug()<<__func__<< "signal cmd :" << sig_cmd;

    if(sig_cmd != REQUEST_SIGNAL)
    {
        qDebug()<<__func__<< "Invalid signal command from SMART CHARGER APP module";
    }

    sprintf(get_json_dbus_payload, "%s", json_object_get_string(jdata.payload));

    qDebug()<<"D-Bus signal payload : " << get_json_dbus_payload;
    qDebug()<<"D-Bus signal payload len : " << strlen(get_json_dbus_payload);
    QString dataQstr(get_json_dbus_payload);
    parseReceivedData(dataQstr);

}

bool DbusConnection::dbus_emitter()
{
    struct dbus_var_t dbus_info;
    char db_payload[DATA_SIZE] = {0};

    dbus_info.obj = LCD_SER_DATA_OBJECT;
    dbus_info.interface = LCD_SER_DATA_INTERFACE;
    dbus_info.sig_name = LCD_SER_DATA_SIG_NAME;

    if(write_init(&dbus_info))
    {
        qDebug()<<"write_init failed ";
        return false;
    }

    pthread_mutex_lock(&data_lock);

    compose_data(&json_data, db_payload, signal_data);
    qDebug()<<"db_payload Check: " << db_payload;

    if(write_data(db_payload, dbus_info))
    {
        qDebug()<<"write_data failed ";
        return false;
    }

    pthread_mutex_unlock(&data_lock);
    signal_data.payload = NULL;
    free(signal_data.payload);

    return true;
}


void DbusConnection::parseReceivedData(QString inputData)
{
    // Parse the JSON string
    json_object* root = json_tokener_parse(inputData.toStdString().c_str());
    if (root == NULL) {
        qDebug()<<"Error parsing JSON.";
        return;
    }

    bool isStatusCmd = inputData.contains("status");
    QString cmd;
    QString type;

    if(isStatusCmd)
    {
        cmd = "status";
        type = "status";
    }
    else
    {
        cmd = "request";
        type = "request";
    }


    // Access the "status/request" object
    json_object* response_obj = json_object_object_get(root, cmd.toStdString().c_str());
    if (response_obj == NULL || !json_object_is_type(response_obj, json_type_object)) {

        qDebug()<<"Error accessing 'request/status' object.";
        json_object_put(root);
        return;
    }

    QString portID, actionType, voltage, current, percentage;


    // Extract values from the JSON object and store them in the struct
    json_object* port_id_obj = json_object_object_get(response_obj, "port_id");
    portID = json_object_get_string(port_id_obj);

    json_object* action_obj = json_object_object_get(response_obj, "action");
    actionType = json_object_get_string(action_obj);

    json_object* voltage_obj = json_object_object_get(response_obj, "voltage");
    voltage = json_object_get_string(voltage_obj);

    if(isStatusCmd)
    {
        json_object* current_obj = json_object_object_get(response_obj, "current");
        current = json_object_get_string(current_obj);

        json_object* percentage_obj = json_object_object_get(response_obj, "percentage");
        percentage = json_object_get_string(percentage_obj);
    }

    //if(selectedPort == portID || portID == "4")
    if(portID == "4")
    {

        emit DBusDataReceived(portID, actionType, voltage, current, percentage, type);
        qDebug()<<"Data emitted to UI for port "<<portID;


    }
    else if(portID == "1")
    {

        emit DBusDataReceived(portID, actionType, voltage, current, percentage, type);
        qDebug()<<"Data emitted to UI for port "<<portID;


    }
    else if(portID == "2")
    {

        emit DBusDataReceived(portID, actionType, voltage, current, percentage, type);
        qDebug()<<"Data emitted to UI for port "<<portID;


    }
    else if(portID == "3")
    {

        emit DBusDataReceived(portID, actionType, voltage, current, percentage, type);
        qDebug()<<"Data emitted to UI for port "<<portID;


    }
    else if(portID == "5")
    {

        emit DBusDataReceived(portID, actionType, voltage, current, percentage, type);
        qDebug()<<"Data emitted to UI for port "<<portID;


    }

    else
    {
        Q_UNUSED(0)
    }



    // Clean up
    json_object_put(root);
}
