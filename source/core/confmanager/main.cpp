#include <QCoreApplication>
#include <QtGlobal>
#include <QException>

#include "confmanager.h"

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
   qCritical("--core    Set core configuration file.");
   qCritical("--device  Set device configuartion file.");
   qCritical("--server  Set RabbitMQ server address.");
}

QString readParameter(QString parameter, QString defaultValue)
{
   int index = QCoreApplication::arguments().indexOf(parameter);
   if (index != -1) {
      return QCoreApplication::arguments().value(index + 1);
   }

   return defaultValue;
}

} // Global

int main(int argc, char* argv[])
{
   const QString DEFAULT_CORE_FILE("../parameters/coreconfig.xml");
   const QString DEFAULT_DEVICE_FILE("../parameters/deviceconfig.xml");
   const QString DEFAULT_SERVER("127.0.0.1");

   qInstallMessageHandler(Global::messageHandler);

   QCoreApplication app(argc, argv);

   // -h or --help can be used to print help
   if (app.arguments().contains("-h") || app.arguments().contains("--help")) {
      Global::printHelp();
   }

   // -v can be used to enable debug and warning prints
   if (app.arguments().contains("-v")) {
       Global::verbose = true;
   }

   // Read optional command line parameters
   QString coreFile = Global::readParameter("--core", DEFAULT_CORE_FILE);
   QString deviceFile = Global::readParameter("--device", DEFAULT_DEVICE_FILE);
   QString serverAddress = Global::readParameter("--server", DEFAULT_SERVER);

   try {
      Core::ConfManager confManager(coreFile, deviceFile, serverAddress);
      return app.exec();
   } catch(QException& e) {
      qCritical("Initialization failed, quitting program.");
   }

   return EXIT_FAILURE;
}
