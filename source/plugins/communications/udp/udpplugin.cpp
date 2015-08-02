#include "udpplugin.h"
#include "udp.h"

namespace Plugins {

const QString UDPPlugin::PLUGIN_NAME("udp");

QString UDPPlugin::name() const
{
   return PLUGIN_NAME;
}

Communication* UDPPlugin::create(Utils::ParameterSet parameters,
                                       QObject* parent)
{
   try {
      return new UDP(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of UDP.");
      return NULL;
   }
}

} // Plugins
