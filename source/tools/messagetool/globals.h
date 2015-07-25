#ifndef GLOBALS_H
#define GLOBALS_H

#include <QObject>

class Globals : QObject {
   Q_OBJECT
   Q_ENUMS(MessageType)
public:
   static const int ENUMINDEX_MESSAGE_TYPE = 0;

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
      LogMessage
   };
};

#endif
