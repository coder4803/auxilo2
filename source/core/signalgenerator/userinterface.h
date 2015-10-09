/* userinterface.h
 * This header defines the UserInterface singleton class.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 9-Oct-2015
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "viewinterface.h"
#include <QCoreApplication>
#include <memory>

namespace SignalGenerator
{

class UserInterface : public ViewInterface
{
public:

    /*!
     * \brief Initializes the UI and creates the QCoreApplication.
     * \param argc Commandline argument count.
     * \param argv Commandline arguments.
     * \return Suitable QCoreApplication on success, else nullptr.
     * \pre argc > 0, argv or none of its elements is null.
     * \post Correct type of QCoreApplication and UserInterface is created.
     *  If initialization fails, nothing has been done.
     */
    static QCoreApplication* initUI(int& argc, char* argv[]);

    /*!
     * \brief Get access to the user interface.
     * \return Instance of user interface.
     * \pre initUI must be successfully called in advance.
     * \post Ownership of the UI is not transfered.
     *  (do NOT delete the returned pointer.)
     */
    static ViewInterface* getInstance();

    /*!
     * \brief Destructor.
     */
    virtual ~UserInterface();

    // Implements the SignalGenerator::ViewInterface.
    virtual void debug(const QString &msg);
    virtual void warning(const QString &msg);
    virtual void critical(const QString &msg);
    virtual void fatal(const QString &msg);
    virtual void setVerbose(bool value);
    virtual void setTableModel(QSqlTableModel *model);


private:

    // Constructor is private.
    UserInterface(ViewInterface *view);

    static QCoreApplication* loadUiPlugin(int& argc, char* argv[]);

    static std::unique_ptr<UserInterface> instance_;
    std::unique_ptr<ViewInterface> view_;

};

} // Namespace SignalGenerator.

#endif // USERINTERFACE_H
