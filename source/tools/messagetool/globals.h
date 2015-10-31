#ifndef GLOBALS_H
#define GLOBALS_H

#include <QObject>

namespace MessageTool {

/*!
 * \brief The Globals class
 * This class holds global staff that is inserted into Qt's meta-object system.
 */
class Globals {
   Q_GADGET
   Q_ENUMS(MessageType)
public:
   //! Enum for message types
   enum MessageType
   {
      SignalMessage,
      SignalAckMessage,
      SetStateMessage,
      SetStateAckMessage,
      RequestStateMessage,
      StateResponseMessage,
      ConfRequestMessage,
      ConfResponseMessage,
      StateChangedMessage,
      StateChangedAckMessage,
      LogMessage,
      EventMessage
   };
};

} // MessageTool

#endif
