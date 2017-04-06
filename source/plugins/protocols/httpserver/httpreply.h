#include "httpmessage.h"

#ifndef HTTPREPLY_H
#define HTTPREPLY_H

/*!
 * \brief Describes HTTP reply message.
 *
 * Class can be used to construct and parse HTTP reply messages.
 */
class HTTPReply : public HTTPMessage
{
public:
   //! Constructor.
   HTTPReply();

   /*!
    * \brief Constructs message with status data.
    *
    * This should be used when constructing HTTP reply.
    *
    * \param protocol Protocol version.
    * \param result Result number.
    * \param resultString Result string.
    */
   HTTPReply(const QString& protocol,
             int result,
             const QString& resultString);

   //! Destructor.
   ~HTTPReply();

   /*!
    * \brief Builds HTTP reply from raw data.
    *
    * This can be used to parse HTTP reply.
    *
    * \param data Message data.
    * \return Number of parsed bytes, -1 on error.
    */
   int buildFromData(const QByteArray &data);

   /*!
    * \brief Returns message's protocol field value.
    * \return Protocol field value.
    */
   QString protocol() const { return m_protocol; }

   /*!
    * \brief Returns message's result field value.
    * \return Result field value.
    */
   int result() const { return m_result; }

   /*!
    * \brief Returns message's result string field value.
    * \return Result string field value.
    */
   QString resultString() const { return m_resultString; }

private:
   //! Protocol field.
   QString m_protocol;

   //! Result field.
   int m_result;

   //! Result string field.
   QString m_resultString;
};

#endif
