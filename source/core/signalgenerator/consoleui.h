/*
 * userinterface.h
 * This header defines the ConsoleUI class, the default user interface.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 10-Oct-2015
 */

#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "viewinterface.h"

namespace SignalGenerator
{

/*!
 * \brief The ConsoleUI class
 *  The console user interface. This is the default user interfece.
 */
class ConsoleUI : public ViewInterface
{
public:

    /*!
     * \brief Constructor.
     */
    ConsoleUI();

    /*!
     * \brief Destructor.
     */
    virtual ~ConsoleUI();

    // Implements the ViewInterface.
    virtual void debug(const QString &msg);
    virtual void warning(const QString &msg);
    virtual void critical(const QString &msg);
    virtual void fatal(const QString &msg);
    virtual void setVerbose(bool value);
    virtual void setModel(ModelInterface *model);


private:

    bool verbose_;
    ModelInterface* model_;
};

} // Namespace SignalGenerator

#endif // CONSOLEUI_H
