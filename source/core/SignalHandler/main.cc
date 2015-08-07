
#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include "signalhandlerbuilder.hh"

bool verbose(false);

void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
    Q_UNUSED(context);
    switch (type) {
    case QtDebugMsg:
    case QtWarningMsg:
        if (verbose){
            printf("%s\n", msg.toLatin1().data());
        }
        break;
        
    case QtCriticalMsg:
       printf("%s\n", msg.toLatin1().data());
       break;
    case QtFatalMsg:
       printf("%s\n", msg.toLatin1().data());
       abort();
    default:
        break;
    }
}

void help()
{
    qCritical("Help:");
    qCritical("-v:        Enable debug messages.");
    qCritical("--server:  Set message server address.");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::unique_ptr<SignalHandler::ModelInterface> model;
    
    if (a.arguments().contains("--help") || a.arguments().contains("-h")){
        help();
    }
    if (a.arguments().contains("-v")){
        verbose = true;
    }
    
    qInstallMessageHandler(messageHandler);
    
    QString address = "127.0.0.1";
    int index = a.arguments().indexOf("--server");
    if (index != -1){
        address = a.arguments().value(index+1);
    }
    
    try {
        model.reset( SignalHandler::SignalHandlerBuilder().create(address) );
        model->start();
    }
    catch (...){
        qCritical("Program initialization failed! Check configuration!");
        a.exit();
    }

    return a.exec();
}
