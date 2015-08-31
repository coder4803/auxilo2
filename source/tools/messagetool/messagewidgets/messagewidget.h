#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <message.h>

namespace MessageTool
{

/*!
 * \brief The MessageWidget class
 *  A base class for Message widgets that are used to insert information needed
 *  to send a message.
 */
class MessageWidget : public QWidget
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \param parent QWidget's parent.
     * \pre None.
     */
    MessageWidget(QWidget* parent = 0) : QWidget(parent) {}

    /*!
     * \brief Destructor
     */
    virtual ~MessageWidget() {}

    /*!
     * \brief Construct a Message object from widget input information
     * \return New Message object. Ownership is passed to the caller. Returns
     *  nullptr, if input information is invalid.
     * \pre None.
     */
    virtual Utils::Message* getMessage() const = 0;

    /*!
     * \brief Get error message, if input information is invalid.
     * \return Error message describing error with input information.
     *  If input information is valid, return empty string.
     * \pre None.
     */
    virtual QString errorString() const = 0;


signals:

    /*!
     * \brief This signal is emitted, when user has accepted message input
     *  information by pressing "OK" on the widget.
     */
    void messageAccepted();
};

}

#endif // MESSAGEWIDGET_H

