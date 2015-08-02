
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

#include "interfaces/viewinterface.hh"
#include "interfaces/modelinterface.hh"
#include <istream>
#include <ostream>
#include <mutex>


/*!
 * \brief The ConsoleUI class
 *  The console user interface.
 * \deprecated This class is deprecated because it does not work in a Qt-app.
 */
class ConsoleUI : public SignalHandler::ViewInterface
{
public:
    
    /*!
     * \brief ConsoleUI Constructor
     * \param input The input stream for user commands.
     * \param output The output stream for messages.
     */
    ConsoleUI(std::istream* input, std::ostream* output);
    
    //! Destructor.
    virtual ~ConsoleUI();
    
    //! Copy- and move-constructors and -assignment operators are forbidden.
    ConsoleUI(const ConsoleUI&) = delete;
    ConsoleUI(ConsoleUI&&) = delete;
    ConsoleUI& operator =(const ConsoleUI&) = delete;
    ConsoleUI& operator =(ConsoleUI&&) = delete;
    
    // Implements the ViewInterface interface.
    virtual void showMessage(const std::string& msg);
    
    /*!
     * \brief exec Start main loop.
     * \return 0 on successful run.
     * \pre -
     * \post UI is responsive.
     */
    int exec();
    
private:
    
    std::istream* input_;
    std::ostream* output_;
    std::recursive_mutex outputMx_;
    SignalHandler::ModelInterface* model_;
};

#endif // CONSOLEUI_HH
