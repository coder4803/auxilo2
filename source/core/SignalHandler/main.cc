
#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include <mutex>
#include "signalhandlerbuilder.hh"
#include "UserInterface/userinterface.hh"


void help()
{
    qCritical("Help:");
    qCritical("-v:        Enable debug messages.");
    qCritical("--server:  Set message server address.");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    // Print help if requested.
    if (a.arguments().contains("--help") || a.arguments().contains("-h")){
        help();
        return 0;
    }
    
    // Create user interface.
    UserInterface::initUI(a.arguments());
    
    // Find message server address.
    QString address = "127.0.0.1";
    int index = a.arguments().indexOf("--server");
    if (index != -1){
        if (a.arguments().length() <= index+1){
            UserInterface::getInstance()->fatal("Invalid commandline argumets: "
                                                "Server address not defined.");
        }
        address = a.arguments().value(index+1);
    }
    
    // Create business logic.
    std::unique_ptr<SignalHandler::ModelInterface> model;
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
