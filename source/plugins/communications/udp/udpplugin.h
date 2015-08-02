#include <QObject>

#include "plugininterfaces.h"

#ifndef UDPPLUGIN_H
#define UDPPLUGIN_H

namespace Plugins {

class UDPPlugin : public QObject, public CommunicationInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.auxilo2.communication.udp")
   Q_INTERFACES(Plugins::CommunicationInterface)

public:
   /*!
    * \brief Returns name of the plugin.
    * \return Name of the plugin.
    */
   QString name() const;

   /*!
    * \brief Creates and returns new instance of UDP communication.
    * \param parameters Communication parameters.
    * \param parent Parent object.
    * \return New instance of communication.
    */
   Communication* create(Utils::ParameterSet parameters,
                         QObject* parent = NULL);

private:
   //! Name of the plugin.
   static const QString PLUGIN_NAME;
};

} // Plugins

#endif
