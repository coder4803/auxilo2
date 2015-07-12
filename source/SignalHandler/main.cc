
#include <QCoreApplication>
#include <memory>
#include "signalhandlerbuilder.hh"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::unique_ptr<SignalHandler::ModelInterface> model;
    model.reset( SignalHandler::SignalHandlerBuilder().create() );
    model->start();
    return a.exec();
}
