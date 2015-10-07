#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "viewinterface.h"

namespace SignalGenerator
{

class ConsoleUI : public ViewInterface
{
public:

    ConsoleUI();

    virtual ~ConsoleUI();

    // Implements the ViewInterface.
    virtual void debug(const QString &msg);
    virtual void warning(const QString &msg);
    virtual void critical(const QString &msg);
    virtual void fatal(const QString &msg);
    virtual void setVerbose(bool value);
    virtual void setTableModel(QSqlTableModel *model);


private:

    bool verbose_;
};

} // Namespace SignalGenerator

#endif // CONSOLEUI_H
