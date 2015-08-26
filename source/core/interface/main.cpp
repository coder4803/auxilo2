#include <QCoreApplication>
#include <QtGlobal>
#include <QException>
#include <signal.h>

#include "interface.h"

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

void handlePOSFIXSignal(int signal)
{
   qCritical("Received signal %i, shutting down...", signal);
   QCoreApplication::quit();
}

} // Global

int main(int argc, char* argv[])
{
   QCoreApplication app(argc, argv);

   qInstallMessageHandler(Global::messageHandler);
   signal(SIGINT, Global::handlePOSFIXSignal);
   signal(SIGTERM, Global::handlePOSFIXSignal);

   // -v can be used to enable debug and warning prints.
   if (app.arguments().contains("-v")) {
       Global::verbose = true;
   }

   // --server can be used to set RabbitMQ server address.
   QString serverAddress("127.0.0.1");
   int serverIndex = app.arguments().indexOf("--server");
   if (serverIndex != -1) {
      serverAddress = app.arguments().value(serverIndex);
   }

   // --feature is used to set interface's feature name.
   int featureIndex = app.arguments().indexOf("--feature");
   if (featureIndex == -1) {
       qCritical("Please use --feature to set interface's feature name.");
       return EXIT_FAILURE;
   }

   if (app.arguments().size() > featureIndex + 1) {
      QString featureName = app.arguments().value(featureIndex + 1);
      Core::Interface interface(serverAddress, featureName);
      return app.exec();
   } else {
      qCritical("Missing feature.");
      return EXIT_FAILURE;
   }
}
