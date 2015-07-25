#include <QCoreApplication>

#include "messagebroker.h"

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);
   Core::MessageBroker messageBroker;
   return a.exec();
}
