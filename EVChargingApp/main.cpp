#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include "DarkStyle.h"
#include "framelesswindow/framelesswindow.h"
#include <qdatetime.h>

// Smart pointer to log file
QScopedPointer<QFile>   m_logFile;
// handler declaration
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Create a QApplication instance

    // Set the logging file
    // check which a path to file you use
    m_logFile.reset(new QFile(QDir::currentPath() + "/AppLogFile.txt"));
    // Open the file logging
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Set handler
    qInstallMessageHandler(messageHandler);

    qDebug() <<"\n"
             <<"\n  #####  #######    #    ######  #######          #     # ######  "
             <<"\n #     #    #      # #   #     #    #             #     # #     # "
             <<"\n #          #     #   #  #     #    #             #     # #     # "
             <<"\n  #####     #    #     # ######     #    #####    #     # ######  "
             <<"\n       #    #    ####### #   #      #             #     # #       "
             <<"\n #     #    #    #     # #    #     #             #     # #       "
             <<"\n  #####     #    #     # #     #    #              #####  #       "
             <<"\n";

    // style our application with custom dark style
    QApplication::setStyle(new DarkStyle);

    // create frameless window (and set windowState or title)
    //FramelessWindow framelessWindow;
    //framelessWindow.setWindowState(Qt::WindowFullScreen);
    //framelessWindow.setWindowTitle("test title");
    //framelessWindow.setWindowIcon(QIcon(":/Icons/Images/icons8-powerline-24.png"));

    // create our mainwindow instance
    MainWindow *mainWindow = new MainWindow;

    // add the mainwindow to our custom frameless window
    //framelessWindow.setContent(mainWindow);

    //framelessWindow.showFullScreen();
    mainWindow->showMinimized();
    int returnStatus =  a.exec();


    qDebug() <<"\n"
             <<"\n  #####  #     # #     # ####### ######  ####### #       # #     # "
             <<"\n #     # #     # #     #    #    #     # #     # #   #   # ##    # "
             <<"\n #       #     # #     #    #    #     # #     # #   #   # # #   # "
             <<"\n  #####  ####### #     #    #    #     # #     # #   #   # #  #  # "
             <<"\n       # #     # #     #    #    #     # #     # #   #   # #   # # "
             <<"\n #     # #     # #     #    #    #     # #     # #   #   # #    ## "
             <<"\n  #####  #     #  #####     #    ######  #######  ### ###  #     # "
             <<"\n";

    return returnStatus;
}

// The implementation of the handler
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Open stream file writes
    QTextStream out(m_logFile.data());
    // Write the date of recording
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Write to the output category of the message and the message itself
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Clear the buffered data
}
