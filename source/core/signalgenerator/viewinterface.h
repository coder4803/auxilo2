/* viewinterface.h
 * This header defines the SignalGenerator::ViewInterface, the interface for
 * all user interfaces.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 7-Oct-2015
 */

#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include <QString>
#include "modelinterface.h"

namespace SignalGenerator
{

/*!
 * \brief The ViewInterface class
 * The absttact interface for user interfaces.
 */
class ViewInterface
{
public:

    /*!
     * \brief Destructor.
     */
    virtual ~ViewInterface() {}

    /*!
     * \brief Display a debug message.
     * \param msg Message to be displayed.
     * \pre None.
     * \post Message is displayed, if UI is in verbose mode.
     */
    virtual void debug(const QString& msg) = 0;

    /*!
     * \brief Display warning message, if UI is verbose.
     * \param msg Message to be displayed.
     * \pre None.
     * \post Message is displayed, if UI is in verbose mode.
     */
    virtual void warning(const QString& msg) = 0;

    /*!
     * \brief Display critical messages.
     * \param msg Message to be displayed.
     * \pre None.
     * \post Message is displayed, even if UI is not in verbose mode.
     */
    virtual void critical(const QString& msg) = 0;

    /*!
     * \brief fatal Display fatal message.
     * \param msg Message to be displayed.
     * \pre None
     * \post Program is terminated after message is displayed.
     */
    virtual void fatal(const QString& msg) = 0;

    /*!
     * \brief Sets UI to verbose or non-verbose mode.
     * \param value Is UI verbose?
     * \pre None.
     * \post If value = true, debug and warning messages are displeyed.
     *  Else they are not.
     */
    virtual void setVerbose(bool value = true) = 0;

    /*!
     * \brief Provide the UI with access to the business logic.
     * \param model Pointer to the model part of program.
     *  Ownership is not passed to the UI.
     * \pre model != nullptr.
     * \post Ui has access to the model.
     */
    virtual void setModel(ModelInterface* model) = 0;
};

} // Namespace SignalGenerator

#endif // VIEWINTERFACE_H

