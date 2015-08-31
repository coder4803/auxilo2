#ifndef MESSAGEWIDGETPOOL_H
#define MESSAGEWIDGETPOOL_H

#include "objectpool.hh"
#include "messagewidget.h"
#include <QString>

namespace MessageTool
{

/*!
 * \brief The MessageWidgetFactory class
 *  Helper class for intantiating PPUtils::ObjectPool.
 */
class MessageWidgetFactory
{
public:

    /*!
     * \brief Factory function.
     * \param Desired MessageWidget type.
     * \return Appropriate subclass of MessageWidget or nullptr, if unknown type.
     * \pre None.
     */
    MessageWidget* operator()(const QString& type);
};


/*!
 * \brief The MessageWidgetSelector class
 *  Helper class for intantiating PPUtils::ObjectPool.
 */
class MessageWidgetSelector
{
public:

    /*!
     * \brief Generate ObjectPool key based on message type.
     * \param Desired type of MessageWidget.
     * \return ObjectPool key.
     * \pre None.
     */
    int operator() (const QString& type);

    /*!
     * \brief Generate ObjectPool key based on object type.
     * \param Released object.
     * \return ObjectPool key.
     * \pre obj != nullptr.
     */
    int operator() (MessageWidget* obj);
};


/*!
 * \brief Type definition for used PPUtils::ObjectPool instantiation.
 */
typedef PPUtils::ObjectPool<MessageWidget, MessageWidgetFactory, int, MessageWidgetSelector> MessageWidgetPool;

}


#endif // MESSAGEWIDGETPOOL_H

