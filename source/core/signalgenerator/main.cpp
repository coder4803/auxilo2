#include <QCoreApplication>
#include "userinterface.h"
#include "businesslogic.h"
#include <memory>

int main(int argc, char *argv[])
{
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
