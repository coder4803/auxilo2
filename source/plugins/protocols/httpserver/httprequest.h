#include "httpmessage.h"

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

/*!
 * \brief Describes HTTP reply message.
 *
 * Class can be used to construct and parse HTTP request messages.
 */
class HTTPRequest : public HTTPMessage
{
public:
    //! Constructor.
    HTTPRequest();

    /*!
     * \brief Constructs request with command info data.
     *
     * This should be used when constructing HTTP request.
     *
     * \param command Command type.
     * \param address Address.
     * \param protocol Protocol version.
     */
    HTTPRequest(const QString& command,
                const QString& address,
                const QString& protocol);

    //! Destructor.
    ~HTTPRequest();

    /*!
     * \brief Builds HTTP request from raw data.
     *
     * This can be used to parse HTTP request.
     *
     * \param data Message data.
     * \return Number of parsed bytes, -1 on error.
     */
    int buildFromData(const QByteArray& data);

    /*!
     * \brief Returns message's command field value.
     * \return Command field value.
     */
    QString command() const { return m_command; }

    /*!
     * \brief Returns message's address field data.
     * \return Address field data.
     */
    QString address() const { return m_address; }

    /*!
     * \brief Returns message's protocol field data.
     * \return Protocol field data.
     */
    QString protocol() const { return m_protocol; }

private:
    //! Command field.
    QString m_command;

    //! Address field.
    QString m_address;

    //! Protocol field.
    QString m_protocol;
};

#endif
