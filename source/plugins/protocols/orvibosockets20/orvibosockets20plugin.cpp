#include "orvibosockets20plugin.h"
#include "orvibosockets20.h"

namespace Plugins {

const QString OrviboSocketS20Plugin::PLUGIN_NAME("orvibosockets20");

QString OrviboSocketS20Plugin::name() const
{
   return PLUGIN_NAME;
}

Protocol* OrviboSocketS20Plugin::create(Utils::ParameterSet parameters,
                                        QObject* parent)
{
   try {
      return new OrviboSocketS20(parameters, parent);
   } catch (QException& e) {
      qCritical("Failed to create instance of OrviboSocketS20.");
      return NULL;
   }
}

} // Plugins
