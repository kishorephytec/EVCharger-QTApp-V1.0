#ifndef DBUS_LISTENER_H
#define DBUS_LISTENER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <dbus/dbus.h>
#include <inc/dbus_interface.h>
#include <inc/dbus_wrapper.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <ctype.h>
#include <errno.h>

#define DATA_SIZE   500

pthread_mutex_t data_lock;

struct signal_data_t signal_data;

#ifdef __cplusplus
}
#endif

#endif
