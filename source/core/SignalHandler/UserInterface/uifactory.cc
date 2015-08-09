#include "uifactory.hh"
// Implementation headers here
// ---------------------------
#include "consoleui.hh"
#include "graphicalui.hh"
// ---------------------------

SignalHandler::ViewInterface*UiFactory::create(const QStringList& args)
{
    bool verbose = false;
    if (args.contains("-v") || args.contains("--verbose")){
        verbose = true;
    }
    
    // Select correct implementation.
    if (args.contains("-g") || args.contains("--graphic")){
        return new GraphicalUI(verbose);
    }
    // ConsoleUI is the default UI.
    return new ConsoleUI(verbose);
}
