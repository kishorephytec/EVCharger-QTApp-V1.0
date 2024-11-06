#ifndef QRFORM_H
#define QRFORM_H

#include <QDialog>

namespace Ui {
class qrform;
}

class qrform : public QDialog
{
    Q_OBJECT

public:
    explicit qrform(QWidget *parent = 0);
    ~qrform();


private slots:
    void on_pushButton_clicked();

private:
    Ui::qrform *ui;
};


#endif // QRFORM_H
