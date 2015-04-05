#include "qtscriptwrapper.hh"
#include <QDebug>

namespace SignalHandler 
{

const char* QtScriptWrapper::LANG_NAME = "QtScript";

QtScriptWrapper::QtScriptWrapper() : ScriptLangWrapper()
{
}

QtScriptWrapper::~QtScriptWrapper()
{   
}

int QtScriptWrapper::run(const QString& script, 
                         const QStringList& args, 
                         ScriptAPI* services)
{
    // Not implemented.
    Q_UNUSED(script);
    Q_UNUSED(args);
    Q_UNUSED(services);
    return 0;
}


QString QtScriptWrapper::getLangID() const
{
    return QString(LANG_NAME);
}


}
