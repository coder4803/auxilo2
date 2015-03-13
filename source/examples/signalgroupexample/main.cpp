#include <QCoreApplication>
#include <QtGlobal>

#include "messager.h"

namespace SignalGroupExample
{
   bool verbose = false;
}

void messageHandler(QtMsgType type,
                    const QMessageLogContext &context,
                    const QString &msg)
{
   Q_UNUSED(context)

   switch (type) {
   case QtDebugMsg:
   case QtWarningMsg:
      if (SignalGroupExample::verbose) {
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

int main(int argc, char *argv[])
{
   qInstallMessageHandler(messageHandler);

   QCoreApplication a(argc, argv);

   // -v can be used to enable debug and warning prints
   if (a.arguments().contains("-v")) {
       SignalGroupExample::verbose = true;
   }

   // Create list of allowed mode parameters
   QStringList allowedParams;
   allowedParams << "-p" << "-s";

   if (a.arguments().size() >= 2) {
      // Check that parameter is on list
      if (allowedParams.contains(a.arguments().at(1))) {
         // Execute application
         Messager messager(a.arguments().at(1));
         return a.exec();
      }
   }

   qCritical("Use either -s (subscriber) or -p (publisher). "
             "Only one is allowed.");

   return 0;
}
