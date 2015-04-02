#ifndef QTSCRIPTWRAPPER_HH
#define QTSCRIPTWRAPPER_HH

#include "scriptlangwrapper.hh"

namespace SignalHandler 
{

class QtScriptWrapper : public ScriptLangWrapper
{
public:
    
    static const char* LANG_NAME;
    
    QtScriptWrapper();
    
    virtual ~QtScriptWrapper();
    
    virtual int run(const QString& script, const QStringList& args);
};


}

#endif // QTSCRIPTWRAPPER_HH
