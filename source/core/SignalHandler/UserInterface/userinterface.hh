#ifndef USERINTERFACE_HH
#define USERINTERFACE_HH

#include "viewinterface.hh"
#include <QStringList>
#include <memory>

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
     * \brief initUI Initialize UserInterface.
     * \param args Commandline arguments. These determine the used UI variation.
     * \pre Call this function once only.
     * \post UserInterface singleton object is ready to be used. Calls to Qt's
     *  printing methods (qDebug, qCritical, qWarning, qFatal) are redirected
     *  to underlaying UI-variation.
     */
    static void initUI(const QStringList& args);
    
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
    
    static std::unique_ptr<UserInterface> instance_;
    std::unique_ptr<ViewInterface> view_;  // The underlaying ViewInterface.
};

#endif // USERINTERFACE_HH
