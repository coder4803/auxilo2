#include "tcpserverplugin.h"
#include "tcpserver.h"

namespace Plugins {

const QString TCPServerPlugin::PLUGIN_NAME("tcpserver");

QString TCPServerPlugin::name() const
{
   return PLUGIN_NAME;
}

Communication* TCPServerPlugin::create(Utils::ParameterSet parameters,
                                       QObject* parent)
{
   try {
      return new TCPServer(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of TCPServer.");
      return NULL;
   }
}

} // Plugins
