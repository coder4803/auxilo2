#include <QStringList>

#include "message.h"

#ifndef UTILS_CONFREQUESTMESSAGE_H
#define UTILS_CONFREQUESTMESSAGE_H

namespace Utils {

/*!
 * \brief The ConfRequestMessage class.
 * This class can be used to create and parse Configuration request messages.
 */
class ConfRequestMessage : public Message
{
public:
   /*!
    * \brief Constructor.
    * This constructor is used to create state message from scratch.
    * \param responseGroup Name of group to send configuration response.
    * \param featureName Name of parameter set.
    */
   ConfRequestMessage(QString responseGroup,
                      QString featureName,
                      bool coreFeature);

   /*!
    * \brief Constructor.
    * This constructor is used to parse state request message from
    * received binary data.
    * \param data Received binary data.
    */
   ConfRequestMessage(QByteArray data);

   /*!
    * \brief Returns response group name.
    * \return Group name to send state response.
    */
   QString responseGroup() const { return m_responseGroup; }

   /*!
    * \brief Returns feature name.
    * \return Feature name.
    */
   const QString& featureName() const { return m_featureName; }

   /*!
    * \brief Tells if requesting core feature.
    * \return True if requesting core feature, otherwise false.
    */
   bool isCoreFeature() const { return m_coreFeature; }

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   QByteArray data() const;

   /*!
    * \brief Returns message in string format.
    * This is useful when debugging.
    * \return Message in string format.
    */
   QString string() const;

private:
   QString m_responseGroup;
   QString m_featureName;
   bool m_coreFeature;
};

} // Utils

#endif
