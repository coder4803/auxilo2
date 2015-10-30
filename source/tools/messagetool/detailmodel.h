#ifndef DETAILMODEL_H
#define DETAILMODEL_H

#include <QAbstractTableModel>

#include "message.h"
#include "messagemodel.h"
#include "globals.h"

namespace MessageTool {

/*!
 * \brief The DetailModel class.
 * This class takes care of parsing and creating messages.
 * This model can be shown with QTableView.
 */
class DetailModel : public QStandardItemModel
{
   Q_OBJECT
public:
   static const int COLUMN_NAME = 0;
   static const int COLUMN_VALUE = 1;
   static const int NUMBER_OF_COLUMNS = 2;

   //! Constructor.
   explicit DetailModel(QObject* parent = NULL);

   //! Destructor.
   ~DetailModel();

   /*!
    * \brief Sets model's raw message data.
    * This method is used to parse raw message.
    * \param messageType Type of the message.
    * \param data Raw data of message.
    */
   void setDetailData(Globals::MessageType messageType,
                      const QByteArray& data);

   /*!
    * \brief Clears model's content.
    */
   void clearContent();

   /*!
    * \brief Controls models editing mode.
    * \param editable Whether model is ductile or not.
    */
   void enableEditing(bool editable);

   /*!
    * \brief Returns field's value options.
    * \param row Row of the detail field.
    * \return List of value options.
    */
   QStringList getOptions(int row);

   /*!
    * \brief Returns text of button for adding rows.
    * \return Text of row add button. Empty if no button is required.
    */
   QString getAddButtonText();

   /*!
    * \brief Returns raw data of message.
    * This method is used to create message.
    * \return Raw data of message.
    */
   QByteArray getMessageData();

public slots:
   /*!
    * \brief Appends empty row that has both columns ductile.
    */
   void addEmptyRow();

private:
   //! Type of the message.
   Globals::MessageType m_messageType;

   //! Whether model is in ductile mode or not.
   bool m_ductile;

   /*!
    * \brief Inserts new row into model.
    * \param name Text for name column.
    * \param value Value for value column.
    * \param options List of legal values for value column.
    * \param ductile Boolean indicator if name column can be edited.
    */
   void newRow(QString name,
               QVariant value,
               QStringList options = QStringList(),
               bool ductile = false);

   // Methods for parsing different messages.
   void parseConfRequestMessage(const QByteArray& data);
   void parseConfResponseMessage(const QByteArray& data);
   void parseSignalMessage(const QByteArray& data);
   void parseSignalAckMessage(const QByteArray& data);
   void parseLogMessage(const QByteArray& data);
   void parseRequestStateMessage(const QByteArray& data);
   void parseStateResponseMessage(const QByteArray& data);
   void parseSetStateMessage(const QByteArray& data);
   void parseSetStateAckMessage(const QByteArray& data);
   void parseStateChangedMessage(const QByteArray& data);
   void parseStateChangedAckMessage(const QByteArray& data);

   // Methods for creating different messages.
   QByteArray createConfRequestMessage();
   QByteArray createConfResponseMessage();
   QByteArray createSignalMessage();
   QByteArray createSignalAckMessage();
   QByteArray createLogMessage();
   QByteArray createRequestStateMessage();
   QByteArray createStateResponseMessage();
   QByteArray createSetStateMessage();
   QByteArray createSetStateAckMessage();
   QByteArray createStateChangedMessage();
   QByteArray createStateChangedAckMessage();
};

} // MessageTool

#endif
