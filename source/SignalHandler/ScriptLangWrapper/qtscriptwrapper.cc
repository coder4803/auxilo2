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

int QtScriptWrapper::run(const QString& script, const QStringList& args)
{
    // Not implemented.
    Q_UNUSED(script);
    Q_UNUSED(args);
    return 0;
}


}
