#include <QObject>

#include "plugininterfaces.h"

#ifndef LESENSORPLUGIN_H
#define LESENSORPLUGIN_H

namespace Plugins {

class LESensorPlugin : public QObject, public ProtocolInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.auxilo2.protocol.lesensorplugin")
   Q_INTERFACES(Plugins::ProtocolInterface)

public:
   /*!
    * \brief Returns name of the plugin.
    * \return Name of the plugin.
    */
   QString name() const;

   /*!
    * \brief Creates and returns new instance of LESensor protocol.
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
