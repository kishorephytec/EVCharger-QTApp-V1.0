#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QThread>
#include <QColor>
#include <qdesktopwidget.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the user interface
    UiInitialization();

    // Establish signal and slot connections
    signalAndSlotConnection();

    // Set the context menu policy to no context menu
    setContextMenuPolicy(Qt::NoContextMenu);

    int width = frameGeometry().width();
    int height = frameGeometry().height();
    QDesktopWidget wid;
    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();
    setGeometry((screenWidth/2)-(width/2),(screenHeight/2)-(height/2),width,height);

}

MainWindow::~MainWindow()
{
    gridPort->StopPortData();
    delete circularProgressBar6;
    delete gridPort;
    delete progressBar;
    delete dateTimer;
    delete GridCheck;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

// Initialize the global data object or variable with an initial value.
void MainWindow::UiInitialization()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_UUTName->setText("Electric Vehicle Charging Station");
    ui->mainToolBar->hide();

    // Hide the label for wrong details
    ui->label_WrongDetails->hide();

    // Set focus on the LoginButton
    ui->LoginButton->setFocus();

    dateTimer = new QTimer(this);
    dateTimer->start(1000);

    GridCheck = new QTimer(this);

    // Create and configure a circular progress bar
    circularProgressBar6 = new CircularProgessBar();
    QConicalGradient conicalGrad;
    conicalGrad.setAngle(90);
    conicalGrad.setColorAt(1.0, Qt::red);
    conicalGrad.setColorAt(0.8, Qt::red);
    conicalGrad.setColorAt(0.6, Qt::yellow);
    conicalGrad.setColorAt(0.4, Qt::yellow);
    conicalGrad.setColorAt(0.2, Qt::green);
    conicalGrad.setColorAt(0, Qt::green);
    circularProgressBar6->setArcPenGradient(conicalGrad);
    ui->horizontalLayout_5->addWidget(circularProgressBar6);

    circularProgressBar6->setProgressValue(0);
    circularProgressBar6->setText("0");

    ui->radioButton_PortA->setChecked(true);
    ui->pushButton_stop->setDisabled(true);
    ui->actionGrid_On_Off->setIconText("Grid Off");

    progressBar = new ProgressWidget(this);
    connect(progressBar, &ProgressWidget::ProgressBarTimeOut, this, &MainWindow::progressBarTimeOut );
}

// Establish connections between signals and appropriate slots
void MainWindow::signalAndSlotConnection()
{
    connect(dateTimer, SIGNAL(timeout()), this, SLOT(showDateAndTime()));
    connect(GridCheck, SIGNAL(timeout()), this, SLOT(GridStatusCheck()));


    // Create DbusConnection objects

    gridPort = new DbusConnection(this);
    connect(gridPort, &DbusConnection::DBusDataReceived, this, &MainWindow::DBusDataReceived );



}

void MainWindow::on_LoginButton_clicked()
{
    userName = ui->lineEdit_userName->text();
    passwd = ui->lineEdit_passWord->text();
    previousGridStatus = "";
    previousPort1Status = "";
    previousPort2Status = "";
    previousPort3Status = "";



    if(userName == "admin" && passwd == "admin")
    {
        ui->label_UUTName->setText("PORTS SELECTION");
        ui->stackedWidget->setCurrentIndex(1);
        ui->pushButton_Proceed->setFocus();
        ui->mainToolBar->show();
        ui->stackedWidget->setDisabled(true);

        gridPort->ReadPortData();

        progressBar->showProgress("Reading Grid Data");

        ui->actionGrid_On_Off->setEnabled(false);
        ui->actionHome->setEnabled(false);
        ui->actionAbout->setEnabled(false);

        return;
    }

    ui->label_WrongDetails->hide();

    if(!(passwd.isEmpty()))
    {
    }
    else
    {
        ui->label_WrongDetails->show();
    }
}


void MainWindow::on_actionHome_triggered()
{


    ui->label_UUTName->setText("PORTS SELECTION");
    ui->stackedWidget->setCurrentIndex(1);
    ui->pushButton_Proceed->setFocus();
    ui->mainToolBar->show();
    gridPort->removeReadPort();

}

// Triggered when the "Logout" action is triggered
void MainWindow::on_actionLogout_triggered()
{
    gridPort->removeReadPort();
    gridPort->StopPortData();

    progressBar->finish(true);
    ui->actionGrid_On_Off->setEnabled(true);
    ui->actionHome->setEnabled(true);
    ui->actionAbout->setEnabled(true);

    ui->stackedWidget->setCurrentIndex(0);
    ui->LoginButton->setFocus();
    ui->mainToolBar->hide();
    ui->stackedWidget->setDisabled(false);
    ui->label_UUTName->setText("Electric Vehicle Charging Station");
}

// Triggered when the "About" action is triggered
void MainWindow::on_actionAbout_triggered()
{
    AboutForm objAbout;
    objAbout.exec();
}

// Triggered when the "Qr" action is triggered
void MainWindow::on_actionQR_triggered()
{
    qrform objqr;
    objqr.exec();
}

// Triggered when the "Proceed" button1 is clicked
void MainWindow::on_pushButton_Proceed_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->pushButton_start->setFocus();
    ui->mainToolBar->show();
    previousPort1Status = "";

    if(ui->radioButton_PortA->isChecked())
    {
        ui->radioButton_PortB->setChecked(false);
        ui->radioButton_PortC->setChecked(false);
        port = "1";

    }
    else
    {
        Q_UNUSED(0)
    }

    //gridPort->addReadPort(port);

    ui->label_UUTName->setText("CHARGING INFORMATION");
    ui->pushButton_port->setText("Port : " + port);

    ui->pushButton_Voltage->setText("Port Voltage : 0 volts");
    ui->pushButton_current->setText("Port Current : 0 amps");
    ui->pushButton_chargingStatus->setText("Status: Charging Stopped");

    ui->pushButton_start->setDisabled(true);
    ui->pushButton_stop->setDisabled(true);

    circularProgressBar6->setProgressValue(0.00);
    circularProgressBar6->setText("0%");
    progressBar->showProgress("Reading port Data");

}
// Triggered when the "Proceed" button2 is clicked
void MainWindow::on_pushButton_Proceed_2_clicked()
{
        ui->stackedWidget->setCurrentIndex(2);
        ui->pushButton_start->setFocus();
        ui->mainToolBar->show();

        previousPort2Status = "";

        if (ui->radioButton_PortB->isChecked())
        {
            ui->radioButton_PortA->setChecked(false);
            ui->radioButton_PortC->setChecked(false);
            port = "2";

        }
        else
        {
            Q_UNUSED(0)
        }

        //gridPort->addReadPort(port);

        ui->label_UUTName->setText("CHARGING INFORMATION");
        ui->pushButton_port->setText("Port : " + port);

        ui->pushButton_Voltage->setText("Port Voltage : 0 volts");
        ui->pushButton_current->setText("Port Current : 0 amps");
        ui->pushButton_chargingStatus->setText("Status: Charging Stopped");

        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setDisabled(true);

        circularProgressBar6->setProgressValue(0.00);
        circularProgressBar6->setText("0%");
        progressBar->showProgress("Reading port Data");
}

// Triggered when the "Proceed" button3 is clicked
void MainWindow::on_pushButton_Proceed_3_clicked()
{
        ui->stackedWidget->setCurrentIndex(2);
        ui->pushButton_start->setFocus();
        ui->mainToolBar->show();
        previousPort3Status = "";

        if (ui->radioButton_PortC->isChecked())
        {
            ui->radioButton_PortB->setChecked(false);
            ui->radioButton_PortA->setChecked(false);
            port = "3";

        }
        else
        {
            Q_UNUSED(0)
        }

        //gridPort->addReadPort(port);

        ui->label_UUTName->setText("CHARGING INFORMATION");
        ui->pushButton_port->setText("Port : " + port);

        ui->pushButton_Voltage->setText("Port Voltage : 0 volts");
        ui->pushButton_current->setText("Port Current : 0 amps");
        ui->pushButton_chargingStatus->setText("Status: Charging Stopped");

        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setDisabled(true);

        circularProgressBar6->setProgressValue(0.00);
        circularProgressBar6->setText("0%");
        progressBar->showProgress("Reading port Data");
}



// Updates the label with the current date and time
void MainWindow::showDateAndTime()
{
    ui->label_dateTime->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy:ddd, h:mm:ss A "));
}

void MainWindow::GridStatusCheck()
{
    ui->actionGrid_On_Off->setIconText("Grid Off");
    ui->stackedWidget->setDisabled(true);
    ui->actionGrid_On_Off->setChecked(false);
    GridCheck->stop();
    progressBarTimeOut("");
}

void MainWindow::on_horizontalSlider_voltage_valueChanged(int value)
{
    ui->spinBox_voltage->setValue(value);
}

void MainWindow::on_spinBox_voltage_valueChanged(int arg1)
{
    ui->horizontalSlider_voltage->setValue(arg1);
}

void MainWindow::on_horizontalSlider_voltage_2_valueChanged(int value)
{
   ui->spinBox_voltage_2->setValue(value);
}


void MainWindow::on_spinBox_voltage_2_valueChanged(int arg1)
{
    ui->horizontalSlider_voltage_2->setValue(arg1);
}


void MainWindow::on_horizontalSlider_voltage_3_valueChanged(int value)
{
    ui->spinBox_voltage_3->setValue(value);
}


void MainWindow::on_spinBox_voltage_3_valueChanged(int arg1)
{
    ui->horizontalSlider_voltage_3->setValue(arg1);
}

void MainWindow::on_pushButton_start_clicked()
{

    if(port == "1")
    {
        previousPort1Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StartCharging(port, QString::number(ui->spinBox_voltage->value()));
        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setDisabled(false);
    }
    else if(port == "2")
    {
        previousPort2Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StartCharging(port, QString::number(ui->spinBox_voltage_2->value()));
        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setDisabled(false);
    }
    else if(port == "3")
    {
        previousPort3Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StartCharging(port, QString::number(ui->spinBox_voltage_3->value()));
        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setDisabled(false);
    }
    else
    {
        Q_UNUSED(0)
    }

}


void MainWindow::on_pushButton_stop_clicked()
{

    if(port == "1")
    {
        previousPort1Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StopCharging(port, QString::number(ui->spinBox_voltage->value()));
        ui->pushButton_start->setDisabled(false);
        ui->pushButton_stop->setDisabled(true);
    }
    else if(port == "2")
    {
        previousPort2Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StopCharging(port, QString::number(ui->spinBox_voltage_2->value()));
        ui->pushButton_start->setDisabled(false);
        ui->pushButton_stop->setDisabled(true);
    }
    else if(port == "3")
    {
        previousPort3Status = "";
        progressBar->showProgress("Reading Port Data");
        gridPort->StopCharging(port, QString::number(ui->spinBox_voltage_3->value()));
        ui->pushButton_start->setDisabled(false);
        ui->pushButton_stop->setDisabled(true);
    }
    else
    {
        Q_UNUSED(0)
    }

}

void MainWindow::progressBarTimeOut(QString text)
{
    EToastr *toastr = new EToastr(this);
    toastr->setStyle(EToastr::TOASTR_STYLE::FAIL);
    toastr->setText("Response signal not received, check hardware connection!");
    toastr->setDuration(ETOSTERTIMEOUT);
    toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);

    ui->actionAbout->setEnabled(true);
    ui->actionLogout->setEnabled(true);
    ui->actionGrid_On_Off->setEnabled(true);
}

void MainWindow::DBusDataReceived(QString portID, QString actionType,
                                  QString voltage, QString current, QString percentage, QString type)
{
    if(portID == "4")
    {   
        updateGridStatus(actionType);
    }
    else if(portID == "1")
    {
        updatePort1Status(portID, actionType, voltage, current, percentage, type);

    }
    else if(portID == "2")
    {
         updatePort2Status(portID, actionType, voltage, current, percentage, type);
    }
    else if(portID == "3")
    {
         updatePort3Status(portID, actionType, voltage, current, percentage, type);
    }

}

void MainWindow::updateGridStatus(QString actionType)
{
    GridCheck->stop();
    GridCheck->start(30 * 1000);

    if(previousGridStatus != actionType)
    {
        ui->actionGrid_On_Off->setEnabled(true);
        ui->actionHome->setEnabled(true);
        ui->actionAbout->setEnabled(true);

        EToastr::TOASTR_STYLE toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
        QString toasterTxt = "";

        if(actionType == "set")
        {
            ui->actionGrid_On_Off->setIconText("Grid On");
            ui->actionGrid_On_Off->setChecked(true);
            ui->stackedWidget->setDisabled(false);

            toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
            toasterTxt = "The grid is turned ON";
        }
        else
        {
            ui->actionGrid_On_Off->setIconText("Grid Off");
            ui->stackedWidget->setDisabled(true);
            ui->actionGrid_On_Off->setChecked(false);

            toastrStyle = EToastr::TOASTR_STYLE::WARNING;
            toasterTxt = "The grid is currently OFF. In order to proceed, please ON the grid.";
        }

        progressBar->finish(true);

        EToastr *toastr = new EToastr(this);
        toastr->setStyle(toastrStyle);
        toastr->setText(toasterTxt);
        toastr->setDuration(ETOSTERTIMEOUT);
        toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);
    }

    previousGridStatus = actionType;
}

void MainWindow::updatePort1Status(QString portID, QString actionType,
                                  QString voltage, QString current, QString percentage, QString type)
{

    GridCheck->stop();
    GridCheck->start(30 * 1000);

    if(type == "status" && portID == port)
    {
        circularProgressBar6->setProgressValue( percentage.toDouble());
        circularProgressBar6->setText(percentage + "%");

        ui->pushButton_current->setText("Port Current : " + current + " amps");
        ui->pushButton_Voltage->setText("Port Voltage : " + voltage + " volts");
    }

    if(type == "request" && actionType == "set")
    {

              int val = voltage.toInt();
              ui->horizontalSlider_voltage->setValue(val);

     }
     else if(type == "request" && actionType == "reset")
     {
                int val = voltage.toInt();
                ui->horizontalSlider_voltage->setValue(val);


     }




    if(previousPort1Status != actionType)
    {
        EToastr::TOASTR_STYLE toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
        QString toasterTxt = "";

        if(actionType == "set")
        {


            ui->pushButton_start->setDisabled(true);
            ui->pushButton_stop->setDisabled(false);
            ui->pushButton_chargingStatus->setText("Status: Charging in progress");
            ui->horizontalSlider_voltage->setDisabled(true);

            ui->spinBox_voltage->setDisabled(true);
            toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
            toasterTxt = "The Port 1 is turned ON";


        }
        else
        {
            ui->pushButton_start->setDisabled(false);
            ui->pushButton_stop->setDisabled(true);
            ui->pushButton_chargingStatus->setText("Status: Charging Stopped");
            ui->horizontalSlider_voltage->setDisabled(false);

            ui->spinBox_voltage->setDisabled(false);
            toastrStyle = EToastr::TOASTR_STYLE::WARNING;
            toasterTxt = "The Port 1 is turned OFF.";

        }


        progressBar->finish(true);

        EToastr *toastr = new EToastr(this);
        toastr->setStyle(toastrStyle);
        toastr->setText(toasterTxt);
        toastr->setDuration(ETOSTERTIMEOUT);
        toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);
    }
    previousPort1Status = actionType;
}


void MainWindow::updatePort2Status(QString portID, QString actionType,
                                  QString voltage, QString current, QString percentage, QString type)
{
    GridCheck->stop();
    GridCheck->start(30 * 1000);


    if(type == "status" && portID == port)
    {
        circularProgressBar6->setProgressValue( percentage.toDouble());
        circularProgressBar6->setText(percentage + "%");

        ui->pushButton_current->setText("Port Current : " + current + " amps");
        ui->pushButton_Voltage->setText("Port Voltage : " + voltage + " volts");
    }
    if(type == "request" && actionType == "set")
    {


              int val = voltage.toInt();
              ui->horizontalSlider_voltage_2->setValue(val);
    }


     else if(type == "request" && actionType == "reset")
     {
                int val = voltage.toInt();
                ui->horizontalSlider_voltage_2->setValue(val);


     }



    if(previousPort2Status != actionType)
    {
        EToastr::TOASTR_STYLE toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
        QString toasterTxt = "";


        if(actionType == "set")
        {


            ui->pushButton_start->setDisabled(true);
            ui->pushButton_stop->setDisabled(false);
            ui->pushButton_chargingStatus->setText("Status: Charging in progress");
            ui->horizontalSlider_voltage_2->setDisabled(true);

            ui->spinBox_voltage_2->setDisabled(true);
            toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
            toasterTxt = "The Port 2 is turned ON";


        }
        else
        {
            ui->pushButton_start->setDisabled(false);
            ui->pushButton_stop->setDisabled(true);
            ui->pushButton_chargingStatus->setText("Status: Charging Stopped");
            ui->horizontalSlider_voltage_2->setDisabled(false);

            ui->spinBox_voltage_2->setDisabled(false);
            toastrStyle = EToastr::TOASTR_STYLE::WARNING;
            toasterTxt = "The Port 2 is turned OFF.";

        }



        progressBar->finish(true);

        EToastr *toastr = new EToastr(this);
        toastr->setStyle(toastrStyle);
        toastr->setText(toasterTxt);
        toastr->setDuration(ETOSTERTIMEOUT);
        toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);
    }
    previousPort2Status = actionType;
}


void MainWindow::updatePort3Status(QString portID, QString actionType,
                                  QString voltage, QString current, QString percentage, QString type)
{
    GridCheck->stop();
    GridCheck->start(30 * 1000);

    if(type == "status" && portID == port)
    {
        circularProgressBar6->setProgressValue( percentage.toDouble());
        circularProgressBar6->setText(percentage + "%");

        ui->pushButton_current->setText("Port Current : " + current + " amps");
        ui->pushButton_Voltage->setText("Port Voltage : " + voltage + " volts");
    }
    if(type == "request" && actionType == "set")
    {

              int val = voltage.toInt();
              ui->horizontalSlider_voltage_3->setValue(val);

    }
    else if(type == "request" && actionType == "reset")
    {
                int val = voltage.toInt();
                ui->horizontalSlider_voltage_3->setValue(val);


    }



    if(previousPort3Status != actionType)
    {

        EToastr::TOASTR_STYLE toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
        QString toasterTxt = "";

        if(actionType == "set")
        {


            ui->pushButton_start->setDisabled(true);
            ui->pushButton_stop->setDisabled(false);
            ui->pushButton_chargingStatus->setText("Status: Charging in progress");
            ui->horizontalSlider_voltage_3->setDisabled(true);

            ui->spinBox_voltage_3->setDisabled(true);
            toastrStyle = EToastr::TOASTR_STYLE::SUCCESS;
            toasterTxt = "The Port 3 is turned ON";


        }
        else
        {
            ui->pushButton_start->setDisabled(false);
            ui->pushButton_stop->setDisabled(true);
            ui->pushButton_chargingStatus->setText("Status: Charging Stopped");
            ui->horizontalSlider_voltage_3->setDisabled(false);

            ui->spinBox_voltage_3->setDisabled(false);
            toastrStyle = EToastr::TOASTR_STYLE::WARNING;
            toasterTxt = "The Port 3 is turned OFF.";

        }



        progressBar->finish(true);

        EToastr *toastr = new EToastr(this);
        toastr->setStyle(toastrStyle);
        toastr->setText(toasterTxt);
        toastr->setDuration(ETOSTERTIMEOUT);
        toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);
    }
    previousPort3Status = actionType;

}

void MainWindow::on_actionGrid_On_Off_triggered(bool checked)
{
    if(checked)
    {
        gridPort->StartGrid();
    }
    else
    {
        gridPort->StopGrid();
    }
    previousGridStatus = "";
    progressBar->showProgress("Reading Grid Data");
}



