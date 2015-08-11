
#include <QApplication>
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
    std::unique_ptr<QCoreApplication> a(nullptr);
    
    // Create user interface.
    a.reset( SignalHandler::UserInterface::initUI(argc, argv) );
    
    if (a->arguments().contains("-h") || a->arguments().contains("--help")){
        help();
        return 0;
    }
    
    // Find message server address.
    QString address = "127.0.0.1";
    int index = a->arguments().indexOf("--server");
    if (index != -1){
        if (a->arguments().length() <= index+1){
            qFatal("Invalid commandline argumets: Server address not defined.");
            return -1;
        }
        address = a->arguments().value(index+1);
    }
    
    // Create business logic.
    std::unique_ptr<SignalHandler::ModelInterface> model;
    try {
        model.reset( SignalHandler::SignalHandlerBuilder().create(address) );
        model->start();
    }
    catch (...){
        qCritical("Program initialization failed! Check configuration!");
        return -1;
    }

    return a->exec();
}
