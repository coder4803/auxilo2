#include <QObject>

#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

namespace Utils {

/*!
 * \brief The Message class
 * Base class for all messages between applications.
 */
class Message : public QObject
{
public:
   /*!
    * \brief Constructor.
    * \param parent Parent object.
    */
   Message(QObject* parent) : QObject(parent) {}

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   virtual QByteArray data() const = 0;

   /*!
    * \brief Returns message in string format.
    * This is usefull when debugging.
    * \return Message in string format.
    */
   virtual QString string() const = 0;
};

} // Utils
#endif
