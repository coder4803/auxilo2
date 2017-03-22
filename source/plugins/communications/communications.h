#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include "plugininterfaces.h"

#define DEFINE_COMMUNICATION_PLUGIN(pluginName) \
    namespace Plugins { \
    \
    class pluginName ## Plugin : public QObject, public CommunicationInterface \
    { \
        Q_OBJECT \
        Q_PLUGIN_METADATA(IID "org.auxilo2.communication." #pluginName "Plugin") \
        Q_INTERFACES(Plugins::CommunicationInterface) \
    \
    public: \
        QString name() const { return #pluginName; } \
        \
        Communication* create(Utils::ParameterSet parameters, \
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
