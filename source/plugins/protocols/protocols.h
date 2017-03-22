#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "plugininterfaces.h"

#define DEFINE_PROTOCOL_PLUGIN(pluginName) \
    namespace Plugins { \
    \
    class pluginName ## Plugin : public QObject, public ProtocolInterface \
    { \
        Q_OBJECT \
        Q_PLUGIN_METADATA(IID "org.auxilo2.protocol." #pluginName "Plugin") \
        Q_INTERFACES(Plugins::ProtocolInterface) \
    \
    public: \
        QString name() const { return #pluginName; } \
        \
        Protocol* create(Utils::ParameterSet parameters, \
                         QObject* parent = NULL) \
        { \
            typedef pluginName pluginName ## Imp; \
            try { \
               return new pluginName ## Imp(parameters, parent); \
            } catch (QException& e) { \
               qCritical("Failed to create instance of " #pluginName  "."); \
               return NULL; \
            } \
        } \
    }; \
    } // Plugins

#endif
