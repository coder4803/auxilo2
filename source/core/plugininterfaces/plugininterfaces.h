#include "communication/communication.h"
#include "protocol/protocol.h"
#include "confresponsemessage.h"

#ifndef PLUGININTERFACES_H
#define PLUGININTERFACES_H

namespace Plugins {

/*!
 * \brief The CommunicationInterface class.
 * This class defines plugin interface for communication plugins.
 */
class CommunicationInterface
{
public:
   //! Destructor
   virtual ~CommunicationInterface() {}

   /*!
    * \brief Returns name of plugin.
    * \return Name of plugin.
    */
   virtual QString name() const = 0;

   /*!
    * \brief Creates instance of communication.
    * \param parameters Parameters for this communication.
    * \param parent Parent object.
    * \return Pointer to instance of communication.
    */
   virtual Communication* create(Utils::ParameterSet parameters,
                                 QObject* parent = NULL) = 0;
};

/*!
 * \brief The ProtocolInterface class.
 * This class defines plugin interface for protocol plugins.
 */
class ProtocolInterface
{
public:
   //! Destructor.
   virtual ~ProtocolInterface() {}

   /*!
    * \brief Returns name of plugin.
    * \return Name of plugin.
    */
   virtual QString name() const = 0;

   /*!
    * \brief Creates instance of protocol.
    * \param parameters Parameters for this protocol.
    * \param parent Parent object.
    * \return Pointer to instance of protocol.
    */
   virtual Protocol* create(Utils::ParameterSet parameters,
                            QObject* parent = NULL) = 0;
};

} // Plugins

Q_DECLARE_INTERFACE(Plugins::CommunicationInterface, "org.auxilo2.communication")
Q_DECLARE_INTERFACE(Plugins::ProtocolInterface, "org.auxilo2.protocol")

#endif
