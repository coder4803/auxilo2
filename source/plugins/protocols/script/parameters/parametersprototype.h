#ifndef PARAMETERSPROTOTYPE_H
#define PARAMETERSPROTOTYPE_H

#include <QByteArray>
#include <QObject>
#include <QScriptable>
#include <QScriptValue>

#include "confresponsemessage.h"

namespace Plugins {

/*!
 * \brief The ParametersPrototype class.
 * This class wraps basic functionality from Utils::ParameterSet for QScript.
 */
class ParametersPrototype : public QObject, public QScriptable
{
   Q_OBJECT
public:
   ParametersPrototype(QObject* parent = 0);
   ~ParametersPrototype();

public slots:
   int count() const;
   bool contains(const QString& key) const;
   QStringList keys() const;
   QScriptValue parameter(const QString& key) const;

private:
   Utils::ParameterSet* thisParameterSet() const;
};

} // Plugins

#endif
