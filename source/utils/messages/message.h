#include <QObject>

#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

class Message : public QObject
{
public:
   Message(QObject* parent) : QObject(parent) {}

   virtual QByteArray data() const = 0;
   virtual QString string() const = 0;
};

#endif
