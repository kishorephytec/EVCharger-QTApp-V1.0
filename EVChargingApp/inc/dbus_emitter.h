#ifndef DBUS_EMITTER_H
#define DBUS_EMITTER_H

#ifdef __cplusplus
extern "C" {
#endif


#define CAN_ID      0x7E0
#define DATA_SIZE   500
#define SIG_DATA    "{\"request\": {\"port_id\": 2, \"action\": \"set\", \"voltage\": 54}}"

#define PORT_A_ON       "{\"request\": {\"port_id\": 1, \"action\": \"set\", \"voltage\": VOLTAGECHANGE}}"
#define PORT_B_ON       "{\"request\": {\"port_id\": 2, \"action\": \"set\", \"voltage\": VOLTAGECHANGE}}"
#define PORT_C_ON       "{\"request\": {\"port_id\": 3, \"action\": \"set\", \"voltage\": VOLTAGECHANGE}}"
#define GRID_ON       "{\"request\": {\"port_id\": 4, \"action\": \"set\", \"voltage\": 0}}"

#define PORT_A_OFF      "{\"request\": {\"port_id\": 1, \"action\": \"reset\", \"voltage\": VOLTAGECHANGE}}"
#define PORT_B_OFF      "{\"request\": {\"port_id\": 2, \"action\": \"reset\", \"voltage\": VOLTAGECHANGE}}"
#define PORT_C_OFF      "{\"request\": {\"port_id\": 3, \"action\": \"reset\", \"voltage\": VOLTAGECHANGE}}"
#define GRID_OFF      "{\"request\": {\"port_id\": 4, \"action\": \"reset\", \"voltage\": 0}}"

struct json_obj_t json_data;

#ifdef __cplusplus
}
#endif

#endif
