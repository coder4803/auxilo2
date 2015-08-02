#include <QObject>

#include "plugininterfaces.h"

#ifndef WEBUIPLUGIN_H
#define WEBUIPLUGIN_H

namespace Plugins {

class WebUIPlugin : public QObject, public ProtocolInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.auxilo2.protocol.webuiplugin")
   Q_INTERFACES(Plugins::ProtocolInterface)

public:
   /*!
    * \brief Returns name of the plugin.
    * \return Name of the plugin.
    */
   QString name() const;

   /*!
    * \brief Creates and returns new instance of WebUI protocol.
    * \param parameters Protocol parameters.
    * \param parent Parent object.
    * \return New instance of protocol.
    */
   Protocol* create(Utils::ParameterSet parameters,
                    QObject* parent = NULL);

private:
   //! Name of the plugin.
   static const QString PLUGIN_NAME;
};

} // Plugins

#endif
