#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include "QProgressIndicator.h"
#include <QWidget>
#include <qtimer.h>
#include <qthread.h>

namespace Ui {
class ProgressWidget;
}

class ProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWidget(QWidget *parent = nullptr);
    ~ProgressWidget();
    void finish(bool success);
    void showProgress(QString text);

public slots:
    void TimerSlot();

signals:
    void ProgressBarTimeOut(QString text);

private:
    Ui::ProgressWidget *ui;
    QTimer *timer;
    QString progressText = "";
};

#endif // PROGRESSWIDGET_H
