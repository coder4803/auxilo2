#include <QStringList>

#include "message.h"

#ifndef UTILS_SIGNALMESSAGE_H
#define UTILS_SIGNALMESSAGE_H

class SignalMessage : public Message
{
public:
   SignalMessage(QString signalName = QString(),
                 QStringList parameters = QStringList(),
                 QObject* parent = NULL);
   SignalMessage(QByteArray data,
                 QObject* parent = NULL);
   ~SignalMessage();

   void setSignalName(QString name);
   void appendParameter(QString value);

   QString signalName() const;
   QString parameter(quint8 index) const;
   QStringList parameters() const;

   QByteArray data() const;
   QString string() const;

private:
   QString m_signalName;
   QStringList m_parameters;
};

#endif
