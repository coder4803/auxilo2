#include <QHash>
#include <QString>
#include <QVariant>
#include <QDebug>

#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

/*!
 * \brief Describes HTTP message.
 */
class HTTPMessage
{
public:
   //! Constructor.
   HTTPMessage();

   //! Destructor
   ~HTTPMessage();

   /*!
    * \brief Builds message object from raw data.
    * \param data Message data.
    * \return Number of parsed bytes, -1 on error.
    */
   virtual int buildFromData(const QByteArray& data);

   /*!
    * \brief Sets first line (request type or status info).
    * \param data First line data.
    */
   void setFirstLine(const QString& data);

   /*!
    * \brief Returns first line data.
    * \return First line data.
    */
   QString firstLine() const { return m_firstLine; }

   /*!
    * \brief Checks if message contains specified header.
    * \param header Name of the header.
    * \return True if header exists, otherwise false.
    */
   bool contains(const QString& header);

   /*!
    * \brief Returns valuea for specified header.
    *
    * Value will be converted to type T.
    *
    * \param name Name of the header.
    * \return Value of the header.
    */
   template <class T> T header(const QString& name) const;

   /*!
    * \brief Sets value for a header.
    * \param name Name of the header field.
    * \param value Value of the header field.
    */
   void setHeader(const QString& name,
                  const QVariant& value);

   /*!
    * \brief Return content field of the message.
    * \return Messages content field.
    */
   QByteArray content() const { return m_content; }

   /*!
    * \brief Sets content field data.
    * \param content Message content.
    */
   void setContent(const QByteArray& content);

   /*!
    * \brief Returns message in raw data format.
    * \return Message in raw data format.
    */
   QByteArray data() const;

private:
   //! Constant for maximum header field length.
   static const int HEADER_FIELD_MAX_LENGTH;

   //! First line data.
   QString m_firstLine;

   //! List of header fields (names and values).
   QHash<QString, QVariant> m_headers;

   //! Message content field.
   QByteArray m_content;
};

template <class T>
T HTTPMessage::header(const QString &name) const
{
   // Header must exist.
   if (!m_headers.contains(name)) {
      qCritical() << name << "doesn't exist!";
      return T();
   }

   QVariant value = m_headers.value(name);

   // Check if value can be converted
   if (!value.canConvert<T>() || !value.convert((int)QVariant(T()).type())) {
      qCritical() << "Value can't be converted to desired type";
      return T();
   }

   // Convert value
   return value.value<T>();
}

#endif
