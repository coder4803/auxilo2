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

} // Global

int main(int argc, char* argv[])
{
   qInstallMessageHandler(Global::messageHandler);

   QCoreApplication app(argc, argv);

   // -v can be used to enable debug and warning prints
   if (app.arguments().contains("-v")) {
       Global::verbose = true;
   }

   try {
      Core::StateHolder stateHolder;
      return app.exec();
   } catch(QException& e) {
      qCritical("Initialization failed, quitting program.");
      return -1;
   }
}
