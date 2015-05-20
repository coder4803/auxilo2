#include "tcpclientplugin.h"
#include "tcpclient.h"

namespace Plugins {

const QString TCPClientPlugin::PLUGIN_NAME("tcpclient");

QString TCPClientPlugin::name() const
{
   return PLUGIN_NAME;
}

Communication* TCPClientPlugin::create(Utils::ParameterSet parameters,
                                       QObject* parent)
{
   try {
      return new TCPClient(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of TCPClient.");
      return NULL;
   }
}

} // Plugins
