
#include <QDebug>
#include <memory>
#include "signalhandlerbuilder.hh"
#include "UserInterface/userinterface.hh"
#include <cstdlib>
#include <algorithm>

void help()
{
    qCritical("Help:");
    qCritical("-v:                     Enable debug messages.");
    qCritical("--server <address>:     Set message server address.");
    qCritical("--ui <UI-plugin name>:  Use custom user interface.");
}


int main(int argc, char *argv[])
{
    // See if 'help' argument is given.
    if (std::find_if(argv+1, argv+argc,
                     [](QString str){return str == "-h" || str =="--help";} )
            != argv+argc)
    {
        help();
        return EXIT_SUCCESS;
    }

    // Create application object and user interface.
    std::unique_ptr<QCoreApplication> a(nullptr);
    a.reset( SignalHandler::UserInterface::initUI(argc, argv) );

    // Find message server address.
    QString address = "127.0.0.1";
    int index = a->arguments().indexOf("--server");
    if (index != -1){
        if (a->arguments().length() <= index+1){
            qFatal("Invalid commandline argumets: Server address not defined.");
            return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    return a->exec();
}
