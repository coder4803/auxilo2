#ifndef SCRIPTAPIADAPTER_H
#define SCRIPTAPIADAPTER_H

#include "scriptapi.hh"

namespace PythonPlugin
{

class ScriptApiAdapter
{
public:

    ScriptApiAdapter(SignalHandler::ScriptAPI* api);


private:

    SignalHandler::ScriptAPI api_;
};

} // Namespace

#endif // SCRIPTAPIADAPTER_H
