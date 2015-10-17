#include <QCoreApplication>
#include "userinterface.h"
#include "businesslogic.h"
#include <memory>

int main(int argc, char *argv[])
{
    if (std::find_if(argv+1, argv+argc,
                     [](char* str){return QString(str)=="--help";})
            != argv+argc)
    {
        qCritical() << "Signal generator is part of the Auxilo2 core.";
        qCritical() << "Signal generator is responsible for timing signals.";
        qCritical() << "Optional commandline arguments:";
        qCritical() << "   -v                     verbose mode.";
        qCritical() << "   --cleardb              clear all previous events.";
        qCritical() << "   --ui <ui name>         Use custom UI.";
        qCritical() << "See developer wiki for more information.";
        return 0;
    }

    std::unique_ptr<QCoreApplication> a(nullptr);
    a.reset( SignalGenerator::UserInterface::initUI(argc, argv) );
    if (a == nullptr){
        return 0;
    }

    bool clear = QCoreApplication::arguments().contains("--cleardb");

    qCritical() << "Starting signalgenerator...";
    SignalGenerator::BusinessLogic logic(clear);
    return a->exec();
}
