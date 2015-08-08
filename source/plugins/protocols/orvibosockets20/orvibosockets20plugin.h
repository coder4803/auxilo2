#include <QObject>

#include "plugininterfaces.h"

#ifndef ORVIBOSOCKETS20PLUGIN_H
#define ORVIBOSOCKETS20PLUGIN_H

namespace Plugins {

class OrviboSocketS20Plugin : public QObject, public ProtocolInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.auxilo2.protocol.orvibosockets20plugin")
   Q_INTERFACES(Plugins::ProtocolInterface)

public:
   /*!
    * \brief Returns name of the plugin.
    * \return Name of the plugin.
    */
   QString name() const;

   /*!
    * \brief Creates and returns new instance of OrviboSocketS20 protocol.
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
