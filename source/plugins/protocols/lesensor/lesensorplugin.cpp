#include "lesensorplugin.h"
#include "lesensor.h"

namespace Plugins {

const QString LESensorPlugin::PLUGIN_NAME("lesensor");

QString LESensorPlugin::name() const
{
   return PLUGIN_NAME;
}

Protocol* LESensorPlugin::create(Utils::ParameterSet parameters,
                                      QObject* parent)
{
   try {
      return new LESensor(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of LESensor.");
      return NULL;
   }
}

} // Plugins
