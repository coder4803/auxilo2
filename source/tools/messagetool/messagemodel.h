#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

/*!
 * \brief The MessageModel class.
 * Instance of message model contains all received messages.
 */
class MessageModel : public QStandardItemModel
{
   Q_OBJECT
public:
   //! Constructor
   explicit MessageModel(QObject *parent = 0);

public slots:
   /*!
    * \brief Inserts new message to the model.
    * \param type Type of the message.
    * \param payload Payload of the message.
    * \param group Name of the group that received the message.
    */
   void onNewMessage(Globals::MessageType type,
                     QByteArray payload,
                     QString group);
};

} // MessageTool

#endif
