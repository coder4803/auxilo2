#ifndef UIFACTORY_HH
#define UIFACTORY_HH

#include "viewinterface.hh"
#include <QStringList>

class UiFactory
{
public:
    
    /*!
     * \brief Create instance of ViewInterface implementation.
     * \param args Commandline parameters.
     * \return ViewInterface. Implementation depends on args.
     */
    static SignalHandler::ViewInterface* create(const QStringList& args);
};

#endif // UIFACTORY_HH
