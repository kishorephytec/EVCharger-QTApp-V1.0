#include "aboutform.h"
#include "qapplication.h"
#include "ui_aboutform.h"
#include <qdebug.h>
#include <qfile.h>

///
/// \brief AboutForm::AboutForm
/// \param parent
///
AboutForm::AboutForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutForm)
{
    // Set up the user interface for the current widget
    ui->setupUi(this);

    // Create a byte array to store data
    QByteArray data;

    // Create a QFile object with the path to the file "EVChargingApp" in the application directory
    QFile file(qApp->applicationDirPath()+"/EVChargingApp");

    // Open the file in read-only mode
    file.open(QIODevice::ReadOnly);

    // Read all data from the file and store it in the byte array
    data = file.readAll();

    // Calculate the checksum of the data using the qChecksum function
    quint16 checksum = qChecksum(data.data(), data.length());

    // Get the HTML content of the text browser widget
    QString htmlData = ui->textBrowser->toHtml();

    // Replace the text "Checksum" with "Checksum - " followed by the calculated checksum value
    htmlData.replace("Checksum","Checksum - "+QString::number(checksum));

    // Clear the content of the text browser widget
    ui->textBrowser->clear();

    // Set the modified HTML content to the text browser widget
    ui->textBrowser->setHtml(htmlData);
}

AboutForm::~AboutForm()
{
    delete ui;
}

void AboutForm::on_pushButton_clicked()
{
    this->accept();
}
