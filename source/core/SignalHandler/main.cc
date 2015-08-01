
#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include "signalhandlerbuilder.hh"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::unique_ptr<SignalHandler::ModelInterface> model;
    
    try {
        model.reset( SignalHandler::SignalHandlerBuilder().create() );
        model->start();
    }
    catch (...){
        qDebug() << "Program initialization failed! Check configuration!";
        a.exit();
    }

    return a.exec();
}
