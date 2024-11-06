#include "progresswidget.h"
#include "ui_progresswidget.h"

#include <qdesktopwidget.h>

ProgressWidget::ProgressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(180,180);

    int width = frameGeometry().width();
    int height = frameGeometry().height();
    QDesktopWidget wid;
    int screenWidth = wid.screen()->width();
    int screenHeight = wid.screen()->height();
    setGeometry((screenWidth/2)-(width/2),(screenHeight/2)-(height/2),width,height);

    ui->progressCircle->setColor(QColor(255,204,0,0.2));

    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),this, SLOT(TimerSlot()));
}

ProgressWidget::~ProgressWidget()
{
    delete timer;
    delete ui;
}

void ProgressWidget::showProgress(QString text)
{
    progressText = text;
    ui->label_Data->setText(progressText);
    this->show();
    ui->progressCircle->startAnimation();
    timer->start(30 * 1000);
}


void ProgressWidget::TimerSlot()
{
    finish(false);
    emit ProgressBarTimeOut(progressText);
}

void ProgressWidget::finish(bool success)
{
    timer->stop();
    ui->progressCircle->stopAnimation();
    this->close();
}
