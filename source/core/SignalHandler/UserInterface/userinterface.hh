#ifndef USERINTERFACE_HH
#define USERINTERFACE_HH

#include "viewinterface.hh"
#include <QCoreApplication>
#include <QStringList>
#include <memory>

namespace SignalHandler
{

/*!
 * \brief The UserInterface class
 *  A Singleton class to access the user interface.
 */
class UserInterface : public SignalHandler::ViewInterface
{
public:
    
    //! Destructor
    virtual ~UserInterface();
    
    //! Copy-constructor is forbidden.
    UserInterface(const UserInterface&) = delete;
    
    //! Copy-assignment is forbidden.
    UserInterface& operator = (const UserInterface&) = delete;
    
    /*!
     * \brief Create instance of QCoreApplication and UserInterface.
     * \param argc Program argument count.
     * \param argv Program argument vector.
     * \return Instance of QCoreApplication that is suitable for the used
     *  ViewInterface. The caller takes the ownership.
     * \pre argc > 0, argv != nullptr. Call this function once only.
     *  There is no other instance of QCoreApplication created previously.
     * \post QCoreApplication object is created. UserInterface is available. 
     *  Qt's print methods are redirected to the used ViewInterface.
     */
    static QCoreApplication* initUI(int& argc, char* argv[]);
    
    /*!
     * \brief Get instance of UserInterface.
     * \return Pointer to the only instance of UserInterface.
     * \pre UserInterface::initUI has been called. Do not delete this pointer.
     *  Ownership is not transfered.
     */
    static UserInterface* getInstance();
    
    // Implements the ViewInterface   
    virtual void critical(const QString& msg);
    virtual void debug(const QString& msg);
    virtual void warning(const QString& msg);
    virtual void fatal(const QString& msg);
    
    
private:
    
    // Constructor is private.
    UserInterface(std::unique_ptr<ViewInterface>&& view);
    
    static QCoreApplication* loadPlugin(int& argc, char* argv[], 
                                        const QString& name, bool verbose);
    
    static std::unique_ptr<UserInterface> instance_;
    std::unique_ptr<ViewInterface> view_;  // The underlaying ViewInterface.
};

} // Namespace SignalHandler.

#endif // USERINTERFACE_HH
