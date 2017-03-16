#include <QCoreApplication>
#include <QtGlobal>
#include <QException>

#include "stateholder.h"

namespace Global {

bool verbose = false;

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &msg)
{
   Q_UNUSED(context)

   switch (type) {
   case QtDebugMsg:
   case QtWarningMsg:
   case QtInfoMsg:
      if (Global::verbose) {
         printf("%s\n", msg.toLatin1().data());
      }
      break;
   case QtCriticalMsg:
      printf("%s\n", msg.toLatin1().data());
      break;
   case QtFatalMsg:
      printf("%s\n", msg.toLatin1().data());
      abort();
   }
}

void printHelp()
{
   qCritical("Help:");
   qCritical("-v        Enable debug and warning messages.");
   qCritical("--server  Set RabbitMQ server address.");
}

} // Global

int main(int argc, char* argv[])
{
   qInstallMessageHandler(Global::messageHandler);

   QCoreApplication app(argc, argv);

   // -v can be used to enable debug and warning prints
   if (app.arguments().contains("-v")) {
       Global::verbose = true;
   }

   // --server can be used to set RabbitMQ server address
   QString serverAddress("127.0.0.1");
   int serverIndex = app.arguments().indexOf("--server");
   if (serverIndex != -1) {
      serverAddress = app.arguments().value(serverIndex);
   }

   Core::StateHolder stateHolder(serverAddress);
   return app.exec();
}
