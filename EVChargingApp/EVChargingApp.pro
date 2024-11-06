#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EVChargingApp
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    DarkStyle.cpp \
    EToastr.cpp \
    QProgressIndicator.cpp \
    aboutform.cpp \
    circularprogessbar.cpp \
    dbusconnection.cpp \
    framelesswindow/framelesswindow.cpp \
    framelesswindow/windowdragger.cpp \
    mainwindow.cpp \
    #qprog.cpp \
    progresswidget.cpp \
    qrform.cpp \
    src/dbus_wrapper.c


HEADERS  += mainwindow.h \
    DarkStyle.h \
    EToastr.h \
    QProgressIndicator.h \
    aboutform.h \
    circularprogessbar.h \
    dbusconnection.h \
    framelesswindow/framelesswindow.h \
    framelesswindow/windowdragger.h \
    inc/dbus_emitter.h \
    inc/dbus_interface.h \
    inc/dbus_listener.h \
    inc/dbus_wrapper.h \
 \    #qprog.h
    progresswidget.h \
    qrform.h

FORMS    += mainwindow.ui \
    Ui/toastr.ui \
    aboutform.ui \
    framelesswindow/framelesswindow.ui \
    progresswidget.ui \
    qrform.ui

RESOURCES += \
    Ui/image.qrc \
    darkstyle.qrc \
    framelesswindow.qrc \
    resource.qrc

# this library are linked as per ubuntu
#LIBS += -pthread
#LIBS += -ldbus-1
#LIBS += -ljson-c

#INCLUDEPATH += /usr/include/dbus-1.0
#DEPENDPATH += /usr/include/dbus-1.0

#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include/
#DEPENDPATH += /usr/lib/x86_64-linux-gnu/dbus-1.0/include/

#INCLUDEPATH += /usr/include/c++/9/
#DEPENDPATH += /usr/include/c++/9/

# this library are linked as per cross platform SDK
LIBS += -pthread
LIBS += -ldbus-1
LIBS += -ljson-c

INCLUDEPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/include/dbus-1.0/
DEPENDPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/include/dbus-1.0/

INCLUDEPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/lib/dbus-1.0/include/
DEPENDPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/lib/dbus-1.0/include/

INCLUDEPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/include/
DEPENDPATH += /opt/phytec-yogurt/BSP-Yocto-i.MX6UL-PD19.1.0/sysroots/cortexa7hf-neon-vfpv4-phytec-linux-gnueabi/usr/include/
