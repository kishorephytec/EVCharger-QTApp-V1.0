#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdebug.h>
#include <qprocess.h>
#include <aboutform.h>
#include <qrform.h>
#include <EToastr.h>
#include "circularprogessbar.h"
#include "dbusconnection.h"
#include <QWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <progresswidget.h>

namespace Ui {
class MainWindow;
}

#define ETOSTERTIMEOUT 4000

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private slots:
    void UiInitialization();
    void signalAndSlotConnection();
    void on_actionLogout_triggered();
    void on_actionAbout_triggered();
    void on_actionQR_triggered();
    void on_pushButton_Proceed_clicked();
    void showDateAndTime();
    void GridStatusCheck();
    void on_spinBox_voltage_valueChanged(int arg1);
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_horizontalSlider_voltage_valueChanged(int value);
    void on_LoginButton_clicked();
    void on_actionHome_triggered();
    void on_actionGrid_On_Off_triggered(bool checked);
    void updateGridStatus(QString actionType);
    void updateModes1(QString portID,QString actionType);
    void updateModes2(QString portID,QString actionType);
    void updatePort1Status(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);
    void updatePort2Status(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);
    void updatePort3Status(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);



    void updatePort5Status(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);

    //void updatePort1Request(QString portID,QString actionType);
   // void updatePort2Request(QString portID,QString actionType);
   // void updatePort3Request(QString portID,QString actionType);



    void on_horizontalSlider_voltage_2_valueChanged(int value);

    void on_spinBox_voltage_2_valueChanged(int arg1);

    void on_horizontalSlider_voltage_3_valueChanged(int value);

    void on_spinBox_voltage_3_valueChanged(int arg1);

    void on_pushButton_Proceed_2_clicked();

    void on_pushButton_Proceed_3_clicked();








    //void on_ModeButton_1_clicked();

    //void on_ModeButton_2_clicked();

    void on_pushButton_Proceed_4_clicked();




    void on_pushButton_Proceed_5_clicked();

    void on_horizontalSlider_voltage_4_valueChanged(int value);

    void on_spinBox_voltage_4_valueChanged(int arg1);

signals:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

public slots:
    void DBusDataReceived(QString portID, QString actionType,
                          QString voltage, QString current, QString percentage, QString type);

    void progressBarTimeOut(QString text);

private:
    Ui::MainWindow *ui;
    QString userName,passwd;

    CircularProgessBar *circularProgressBar6;
    DbusConnection *gridPort;
    ProgressWidget *progressBar;
    QTimer *dateTimer;
    QTimer *GridCheck;

    QString port = "";
    QString previousGridStatus = "";
   // QString previousPortStatus = "";
    QString previousPort1Status = "";
    QString previousPort2Status = "";
    QString previousPort3Status = "";
    QString previousMode2status = "";
    double time;
};

#endif // MAINWINDOW_H
