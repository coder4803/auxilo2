#ifndef PARAMETERSCLASS_H
#define PARAMETERSCLASS_H

#include <QObject>
#include <QScriptClass>
#include <QScriptString>
#include <QScriptContext>

#include "confresponsemessage.h"

namespace Plugins {

/*!
 * \brief The ParametersClass class.
 * This class handles properties for Utils::ParameterSet in Qt scripts.
 */
class ParametersClass : public QObject, public QScriptClass
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor
     * \param engine "Linked/parent" QScriptEngine.
     */
    ParametersClass(QScriptEngine *engine);

    //! Destructor
    ~ParametersClass();

    /*!
     * \brief Returns constructor for script.
     * \return Class script constructor.
     */
    QScriptValue constructor() const;

    /*!
     * \brief Returns name of this class.
     * \return Name of this class.
     */
    QString name() const;

    /*!
     * Overloaded method to handle query property of Parameters class.
     * Basically this method denies write-access and only allow to
     * handle existing properties (parameter names).
     */
    QueryFlags queryProperty(const QScriptValue &object,
                             const QScriptString &name,
                             QueryFlags flags, uint *id);

    /*!
     * Overloaded method to handle read-access of Parameters class.
     * Basically this method returns parameter value by key (property name).
     */
    QScriptValue property(const QScriptValue &object,
                          const QScriptString &name, uint id);

    /*!
     * Overloaded method to handle property flags of Parameter class.
     * Basically this method returns readOnly and undeletable for
     * all properties.
     */
    QScriptValue::PropertyFlags propertyFlags(
        const QScriptValue &object, const QScriptString &name, uint id);

private:
    /*!
     * \brief Returns prototype of this class.
     * \return Prototype of this class.
     */
    QScriptValue prototype() const;

    /*!
     * \brief Creates new instance of Parameters for script.
     * \param ps ParameterSet to create instance from.
     * \return Parameter instance as QScriptValue.
     */
    QScriptValue newInstance(const Utils::ParameterSet& ps);

    //! Method for constructing Parameter instance.
    static QScriptValue construct(QScriptContext *context,
                                  QScriptEngine *eng);

    //! Method for converting ParameterSet to Parameter.
    static QScriptValue toScriptValue(QScriptEngine *eng,
                                      const Utils::ParameterSet &ps);

    //! Method for converting Parameter to ParameterSet.
    static void fromScriptValue(const QScriptValue &obj,
                                Utils::ParameterSet &ps);

    //! Prototype for this class.
    QScriptValue m_prototype;

    //! Script constructor for this class.
    QScriptValue m_constructor;
};

} // Plugins

#endif
