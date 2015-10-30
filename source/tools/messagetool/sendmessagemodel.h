#ifndef SENDMODEL_H
#define SENDMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

/*!
 * \brief The SendMessageModel class.
 * This class represents model messages ready to send.
 */
class SendMessageModel : public QStandardItemModel
{
   Q_OBJECT
public:
   // Labels for model columns.
   static const int INDEX_TARGET_GROUP = 0;
   static const int INDEX_DESCRIPTION = 1;
   static const int INDEX_PAYLOAD = 2;
   static const int NUMBER_OF_COLUMNS = 3;

   //! Constructor.
   explicit SendMessageModel(QObject *parent = 0);

   //! Adds new (empty/default) message.
   void newMessage();

   //! Removes selected message.
   bool removeMessage(int row);

   /*!
    * \brief Sets target group of given message.
    * \param row Row of the message.
    * \param targetGroup Name of target group.
    * \return True if success, otherwise false.
    */
   bool setTargetGroup(int row, QString targetGroup);

   /*!
    * \brief Sets description of given message.
    * \param row Row of the message.
    * \param description Description of the message.
    * \return True if success, otherwise false.
    */
   bool setDescription(int row, QString description);

   /*!
    * \brief Sets payload of given message.
    * \param row Row of the message.
    * \param payload Payload of the message.
    * \return True if success, otherwis false.
    */
   bool setPayload(int row, QByteArray payload);

   /*!
    * \brief Returns target group of given message.
    * \param row Row of the message.
    * \return Name of target group.
    */
   QString getGroup(int row);

   /*!
    * \brief Returns description of given message.
    * \param row Row of the message.
    * \return Description of the message.
    */
   QString getDescription(int row);

   /*!
    * \brief Returns payload of given message.
    * \param row Row of the message.
    * \return Paylod of the message.
    */
   QByteArray getPayload(int row);
};

} // MessageTool

#endif
