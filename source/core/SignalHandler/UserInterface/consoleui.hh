
/* consoleui.hh
 * 
 * This header defines the ConsoleUI class - Console user interface.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-April-2015
 * Last modified: 06-April-2015
 */

#ifndef CONSOLEUI_HH
#define CONSOLEUI_HH

#include "viewinterface.hh"
#include <mutex>


/*!
 * \brief The ConsoleUI class
 *  The console user interface.
 */
class ConsoleUI : public SignalHandler::ViewInterface
{
public:
    
    /*!
     * \brief ConsoleUI Constructor
     * \param input The input stream for user commands.
     * \param output The output stream for messages.
     */
    ConsoleUI(bool verbose);
    
    //! Destructor.
    virtual ~ConsoleUI();
    
    //! Copy- and move-constructors and -assignment operators are forbidden.
    ConsoleUI(const ConsoleUI&) = delete;
    ConsoleUI(ConsoleUI&&) = delete;
    ConsoleUI& operator =(const ConsoleUI&) = delete;
    ConsoleUI& operator =(ConsoleUI&&) = delete;
    
    // Implements the ViewInterface interface.
    
    /*!
     * \brief Displays msg wheather 'verbose' is set or not.
     *  This is equivalent to qCritical() << msg.
     * \param msg Message to be displayed.
     * \pre None.
     */
    virtual void critical(const QString& msg);
    
    /*!
     * \brief Display debug message.
     * \param msg Message.
     * \pre None.
     * \post Displays message if 'verbose' is set. This is equivalent to
     *  qDebug() << msg. 
     */
    virtual void debug(const QString& msg);
    
    /*!
     * \brief Display warning message
     * \param msg Message
     * \pre None.
     * \post Display msg if 'verbose' is set. This is equivalent to
     *  qWarning() << msg;
     */
    virtual void warning(const QString& msg);
    
    /*!
     * \brief Display fatal message and abort program.
     * \param msg Message.
     * \pre None
     * \post Equivalent to qFatal(msg.toLatin1().data());
     */
    virtual void fatal(const QString& msg);
    
};

#endif // CONSOLEUI_HH
