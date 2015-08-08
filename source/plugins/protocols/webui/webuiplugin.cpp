#include "webuiplugin.h"
#include "webui.h"

namespace Plugins {

const QString WebUIPlugin::PLUGIN_NAME("webui");

QString WebUIPlugin::name() const
{
   return PLUGIN_NAME;
}

Protocol* WebUIPlugin::create(Utils::ParameterSet parameters,
                                      QObject* parent)
{
   try {
      return new WebUI(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of WebUI.");
      return NULL;
   }
}

} // Plugins
