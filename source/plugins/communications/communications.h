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
           qCritical() << "Failed to create instance of " #pluginName "."; \
           return NULL; \
         } \
      } \
   }; \
   } // Plugins

#define DEFINE_SHARED_COMMUNICATION_PLUGIN(pluginName, keyParameter) \
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
         if (parameters.contains(#keyParameter)) { \
            QString key = parameters.parameter<QString>(#keyParameter); \
            QMutexLocker locker(&m_instanceMutex); \
            if (!m_instances.contains(key)) { \
               Communication* connection = NULL; \
               try { \
                  connection = new pluginName ## Imp(parameters, parent); \
               } catch (QException& e) { \
                  qCritical() << "Failed to create instance of " \
                            #pluginName "."; \
                  return NULL; \
               } \
               m_instances.insert(key, connection); \
            } \
            return m_instances.value(key); \
         } else { \
            qCritical() << "Missing key parameter: " #keyParameter; \
            return NULL; \
         } \
      } \
   private: \
      QHash<QString, Communication*> m_instances; \
      QMutex m_instanceMutex; \
   }; \
   } // Plugins

#endif
